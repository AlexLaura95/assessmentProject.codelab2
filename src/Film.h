#pragma once
#include "ofMain.h"

class Film {
public:
    std::string title;
    std::string releaseDate;
    std::string overview;
    float rating;
    std::string posterPath;
    
    ofImage poster;
    bool hasPoster;

    Film();
    Film(std::string t, std::string r, std::string o, float score, std::string p = "");

    void drawCard(float x, float y, float width, float height) const;
};