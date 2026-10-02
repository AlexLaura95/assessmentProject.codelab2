#pragma once
#include "ofMain.h"
#include "Film.h"
#include <algorithm>

// Reusable button component with hover feedback
struct AppButton {
    ofRectangle rect;
    std::string label;
    ofColor normalColor;
    ofColor hoverColor;

    bool isHovered(int mx, int my) const {
        return rect.inside(mx, my);
    }

    void draw(int mx, int my) const {
        if (isHovered(mx, my)) {
            ofSetColor(hoverColor);
        } else {
            ofSetColor(normalColor);
        }
        ofDrawRectRounded(rect, 6);

        // Center the button label text
        ofSetColor(255);
        float textX = rect.x + (rect.width - (label.length() * 8.0f)) / 2.0f;
        float textY = rect.y + (rect.height / 2.0f) + 4.0f;
        ofDrawBitmapString(label, textX, textY);
    }
};

class ofApp : public ofBaseApp {
public:
    void setup() override;
    void update() override;
    void draw() override;
    void keyPressed(int key) override;
    void mousePressed(int x, int y, int button) override;

    void searchMovies(const std::string& query);

    std::vector<Film> films;
    std::string currentQuery;
    std::string statusMessage;
    std::string apiKey;

    // Interactive UI Buttons
    AppButton searchBtn;
    AppButton clearBtn;
    AppButton sortRatingBtn;
    AppButton sortDateBtn;
    AppButton sortTitleBtn;
};