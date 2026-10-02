#include "ofApp.h"

void ofApp::setup() {
    ofSetWindowTitle("Film Finder - TMDb");
    ofBackground(13, 17, 23); // Deep slate background

    apiKey = "9d0ad53463d2b951a9cb93c47e56156d";
    currentQuery = "Spiderman";
    statusMessage = "Ready. Click Search or press Enter.";

    // 1. Top action buttons: [SEARCH] (Green) and [CLEAR] (Muted Red)
    searchBtn = { ofRectangle(615, 20, 100, 48), "SEARCH", ofColor(35, 134, 54), ofColor(46, 160, 67) };
    clearBtn  = { ofRectangle(725, 20, 90, 48),  "CLEAR",  ofColor(110, 41, 41), ofColor(164, 46, 46) };

    // 2. Filter / Sort pills: [Top Rated], [Newest], [Title A-Z]
    sortRatingBtn = { ofRectangle(130, 80, 105, 28), "Top Rated", ofColor(33, 38, 45), ofColor(56, 139, 253) };
    sortDateBtn   = { ofRectangle(245, 80, 90, 28),  "Newest",    ofColor(33, 38, 45), ofColor(56, 139, 253) };
    sortTitleBtn  = { ofRectangle(345, 80, 100, 28), "Title A-Z", ofColor(33, 38, 45), ofColor(56, 139, 253) };

    // Initial search
    searchMovies(currentQuery);
}

void ofApp::searchMovies(const std::string& query) {
    if (apiKey.empty()) {
        statusMessage = "Notice: Insert your TMDb API key in ofApp.cpp";
        return;
    }

    films.clear();
    statusMessage = "Searching for: " + query + "...";

    std::string encodedQuery = "";
    for (char c : query) {
        if (c == ' ') encodedQuery += "%20";
        else encodedQuery += c;
    }

    std::string url = "https://api.themoviedb.org/3/search/movie?api_key=" + apiKey + "&query=" + encodedQuery;
    ofHttpResponse response = ofLoadURL(url);

    if (response.status != 200) {
        statusMessage = "API Error: HTTP Status " + ofToString(response.status);
        return;
    }

    try {
        ofJson json = ofJson::parse(response.data);

        if (json.contains("results") && json["results"].is_array()) {
            for (const auto& item : json["results"]) {
                std::string title = item.value("title", "Unknown");
                std::string date = item.value("release_date", "N/A");
                std::string overview = item.value("overview", "No plot provided.");
                float score = item.value("vote_average", 0.0f);

                std::string posterPath = "";
                if (item.contains("poster_path") && item["poster_path"].is_string()) {
                    posterPath = item["poster_path"].get<std::string>();
                }

                films.emplace_back(title, date, overview, score, posterPath);
            }
        }

        // Fetch official poster images for the first 5 movies
        for (size_t i = 0; i < films.size() && i < 5; ++i) {
            if (!films[i].posterPath.empty()) {
                std::string imgUrl = "https://image.tmdb.org/t/p/w185" + films[i].posterPath;
                ofHttpResponse imgResp = ofLoadURL(imgUrl);
                if (imgResp.status == 200) {
                    films[i].poster.load(imgResp.data);
                    films[i].hasPoster = true;
                }
            }
        }

        if (films.empty()) {
            statusMessage = "No movies found matching: " + query;
        } else {
            statusMessage = "Found " + ofToString(films.size()) + " results for: " + query;
        }
    } catch (const std::exception& e) {
        statusMessage = "JSON Parsing Error: " + std::string(e.what());
    }
}

void ofApp::update() {}

void ofApp::draw() {
    // 1. Search Input Bar Box
    ofSetColor(22, 27, 34);
    ofDrawRectRounded(20, 20, 580, 48, 8);

    ofNoFill();
    ofSetColor(88, 166, 255); // Subtle cyan-blue border
    ofDrawRectRounded(20, 20, 580, 48, 8);
    ofFill();

    ofSetColor(88, 166, 255);
    ofDrawBitmapString("SEARCH:", 36, 49);

    ofSetColor(255, 255, 255);
    ofDrawBitmapString(currentQuery + "_", 102, 49);

    // 2. Action Buttons (Search & Clear)
    searchBtn.draw(mouseX, mouseY);
    clearBtn.draw(mouseX, mouseY);

    // 3. Sorting & Filtering Bar
    ofSetColor(139, 148, 158);
    ofDrawBitmapString("Sort Filter:", 24, 98);
    sortRatingBtn.draw(mouseX, mouseY);
    sortDateBtn.draw(mouseX, mouseY);
    sortTitleBtn.draw(mouseX, mouseY);

    // 4. Status Line
    ofSetColor(139, 148, 158);
    ofDrawBitmapString(statusMessage, 470, 98);

    // 5. Render Film Result Cards
    float cardY = 122.0f;
    float cardWidth = ofGetWidth() - 40.0f;
    float cardHeight = 115.0f;

    for (size_t i = 0; i < films.size() && i < 5; ++i) {
        films[i].drawCard(20.0f, cardY, cardWidth, cardHeight);
        cardY += cardHeight + 14.0f;
    }
}

void ofApp::mousePressed(int x, int y, int button) {
    // Check if user clicked SEARCH button
    if (searchBtn.isHovered(x, y)) {
        searchMovies(currentQuery);
    }
    // Check if user clicked CLEAR button
    else if (clearBtn.isHovered(x, y)) {
        currentQuery.clear();
        films.clear();
        statusMessage = "Search cleared. Type a movie name.";
    }
    // Sort: Highest rating first
    else if (sortRatingBtn.isHovered(x, y)) {
        std::sort(films.begin(), films.end(), [](const Film& a, const Film& b) {
            return a.rating > b.rating;
        });
        statusMessage = "Sorted by rating (highest first).";
    }
    // Sort: Newest release date first
    else if (sortDateBtn.isHovered(x, y)) {
        std::sort(films.begin(), films.end(), [](const Film& a, const Film& b) {
            return a.releaseDate > b.releaseDate;
        });
        statusMessage = "Sorted by release date (newest first).";
    }
    // Sort: Alphabetical title (A-Z)
    else if (sortTitleBtn.isHovered(x, y)) {
        std::sort(films.begin(), films.end(), [](const Film& a, const Film& b) {
            return a.title < b.title;
        });
        statusMessage = "Sorted alphabetically (A-Z).";
    }
}

void ofApp::keyPressed(int key) {
    if (key == OF_KEY_RETURN) {
        searchMovies(currentQuery);
    } else if (key == OF_KEY_BACKSPACE) {
        if (!currentQuery.empty()) currentQuery.pop_back();
    } else if (key >= 32 && key <= 126) {
        currentQuery += static_cast<char>(key);
    }
}