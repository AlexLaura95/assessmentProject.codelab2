#include "Film.h"

Film::Film() {
    title = "Unknown";
    releaseDate = "N/A";
    overview = "No description available.";
    rating = 0.0f;
    posterPath = "";
    hasPoster = false;
}

Film::Film(std::string t, std::string r, std::string o, float score, std::string p) {
    title = t;
    releaseDate = r;
    overview = o;
    rating = score;
    posterPath = p;
    hasPoster = false;
}

void Film::drawCard(float x, float y, float width, float height) const {
    // 1. Card Container & Subtle Border
    ofSetColor(26, 31, 40);
    ofDrawRectRounded(x, y, width, height, 8);

    ofNoFill();
    ofSetColor(48, 54, 66);
    ofDrawRectRounded(x, y, width, height, 8);
    ofFill();

    // 2. Poster Image (64x95 matches standard 2:3 poster aspect ratio)
    float thumbW = 64.0f;
    float thumbH = height - 20.0f;

    if (hasPoster && poster.isAllocated()) {
        ofSetColor(255, 255, 255); // Reset color tint to full brightness
        poster.draw(x + 10, y + 10, thumbW, thumbH);
    } else {
        ofSetColor(18, 22, 28);
        ofDrawRectRounded(x + 10, y + 10, thumbW, thumbH, 6);
        ofSetColor(110, 118, 129);
        ofDrawBitmapString("NO\nIMAGE", x + 16, y + (height / 2.0f));
    }

    // 3. Movie Title & Release Year
    float textLeft = x + thumbW + 20.0f;
    std::string year = (releaseDate.length() >= 4) ? releaseDate.substr(0, 4) : "N/A";

    ofSetColor(255, 255, 255);
    ofDrawBitmapStringHighlight(title, textLeft, y + 26, ofColor(13, 17, 23), ofColor(240, 246, 252));

    ofSetColor(139, 148, 158);
    ofDrawBitmapString("(" + year + ")", textLeft + (title.length() * 8) + 12, y + 26);

    // 4. Rating Pill
    float badgeX = textLeft;
    float badgeY = y + 40.0f;

    if (rating >= 7.0f) {
        ofSetColor(46, 160, 67);
    } else if (rating >= 5.0f) {
        ofSetColor(210, 153, 34);
    } else {
        ofSetColor(207, 34, 46);
    }
    ofDrawRectRounded(badgeX, badgeY, 95, 18, 4);

    ofSetColor(255, 255, 255);
    ofDrawBitmapString("Score: " + ofToString(rating, 1), badgeX + 8, badgeY + 13);

    // 5. Plot Overview Snippet
    ofSetColor(201, 209, 217);
    std::string snippet = overview;
    if (snippet.length() > 210) {
        snippet = snippet.substr(0, 207) + "...";
    }

    if (snippet.length() > 105) {
        size_t splitIndex = snippet.rfind(' ', 105);
        if (splitIndex == std::string::npos) splitIndex = 105;
        ofDrawBitmapString(snippet.substr(0, splitIndex), textLeft, y + 76);
        ofDrawBitmapString(snippet.substr(splitIndex + 1), textLeft, y + 94);
    } else {
        ofDrawBitmapString(snippet, textLeft, y + 80);
    }
}