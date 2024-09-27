#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <filesystem>
#include <string>
#include <iostream>

using namespace cv;
using namespace std;

// Function to convert to black and white (grayscale)
void convertToBlackAndWhite(Mat& image) {
    cvtColor(image, image, COLOR_BGR2GRAY);
    cvtColor(image, image, COLOR_GRAY2BGR); // Convert back to 3 channels after grayscale
}

// Function to set high saturation
void setHighSaturation(Mat& image) {
    Mat hsvImage;
    cvtColor(image, hsvImage, COLOR_BGR2HSV);

    for (int y = 0; y < hsvImage.rows; ++y) {
        for (int x = 0; x < hsvImage.cols; ++x) {
            hsvImage.at<Vec3b>(y, x)[1] = 255; // Max saturation
        }
    }

    cvtColor(hsvImage, image, COLOR_HSV2BGR);
}

// Function to change hue
void changeHue(Mat& image, int hueShift) {
    Mat hsvImage;
    cvtColor(image, hsvImage, COLOR_BGR2HSV);

    for (int y = 0; y < hsvImage.rows; ++y) {
        for (int x = 0; x < hsvImage.cols; ++x) {
            int newHue = hsvImage.at<Vec3b>(y, x)[0] + hueShift;
            newHue = (newHue % 180 + 180) % 180; // Keep hue within [0, 180] range
            hsvImage.at<Vec3b>(y, x)[0] = newHue;
        }
    }

    cvtColor(hsvImage, image, COLOR_HSV2BGR);
}

// Function to change saturation
void changeSaturation(Mat& image, int saturationShift) {
    Mat hsvImage;
    cvtColor(image, hsvImage, COLOR_BGR2HSV);

    for (int y = 0; y < hsvImage.rows; ++y) {
        for (int x = 0; x < hsvImage.cols; ++x) {
            int newSaturation = hsvImage.at<Vec3b>(y, x)[1] + saturationShift;
            newSaturation = min(255, max(0, newSaturation)); // Clamp to [0, 255]
            hsvImage.at<Vec3b>(y, x)[1] = newSaturation;
        }
    }

    cvtColor(hsvImage, image, COLOR_HSV2BGR);
}

// Function to add noise
void addNoise(Mat& image, int noiseLevel) {
    Mat noise(image.size(), image.type());
    randn(noise, 0, noiseLevel);  // Gaussian noise
    image += noise;
}

// Function to apply Gaussian blur
void applyGaussianBlur(Mat& image, int kernelSize = 5) {
    GaussianBlur(image, image, Size(kernelSize, kernelSize), 0);
}

// Function to combine two textures
void combineTextures(const Mat& image1, const Mat& image2, Mat& output, double alpha = 0.5) {
    addWeighted(image1, alpha, image2, 1.0 - alpha, 0.0, output);
}

// Function to generate variations for one texture
void generateVariations(const Mat& baseTexture, const vector<Mat>& allTextures, int textureIndex, int variationCount, const string& textureName, const string& outputDir) {
    for (int i = 0; i < variationCount; ++i) {
        Mat variation = baseTexture.clone();

        // Apply hue shift, saturation shift, noise, and blur
        int hueShift = (i * 7) % 180; // Hue variation
        int saturationShift = (i * 10) % 100; // Saturation variation
        int noiseLevel = (i * 30) % 100; // Noise level variation
        int blurKernel = (i % 3 == 0) ? 11 : 9; // Blur variation

        changeHue(variation, hueShift);
        changeSaturation(variation, saturationShift);
        addNoise(variation, noiseLevel);
        applyGaussianBlur(variation, blurKernel);

        // Combine two random textures every 5th variation
        if (i % 5 == 0) {
            int randomTextureIdx = rand() % allTextures.size();
            Mat combinedTexture;
            combineTextures(variation, allTextures[randomTextureIdx], combinedTexture, 0.5);
            variation = combinedTexture;
        }

        // Black and white variation every 6th variation
        if (i % 6 == 0) {
            convertToBlackAndWhite(variation);
        }

        // High saturation variation every 7th variation
        if (i % 7 == 0) {
            setHighSaturation(variation);
        }

        string filename = outputDir + textureName + "_Variation_" + to_string(i) + ".png";
        imwrite(filename, variation);
    }
}

int main() {
    // Load the 50 base textures
    vector<Mat> baseTextures;

    /*
    vector<string> textureName = {
        "AcaciaSaloon_Roof_1.1.png",
        "BrownPlaster_Wall_1.0.png",
        "Cement_Wall_1.0.png",
        "Cloth1_Clothing_1.0.png",
        "Cloth2_Clothing_1.0.png",
        "Cloth3_Clothing_1.0.png",
        "Cloth4_Clothing_1.0.png",
        "Cloth5_Clothing_1.0.png",
        "Cloth6_Clothing_1.0.png",
        "Cloth7_Clothing_1.0.png",
        "Cloth8_Clothing_1.0.png",
        "Cloth9_Clothing_1.0.png",
        "DamascusSteel_Prop_1.0.png",
        "Fiberglass_Ceiling_1.1.png",
        "HexagonpatternFloor_Floor_1.0.png",
        "MatteGreenMesh_Prop_1.0.png",
        "MetalSheet_Roof_1.1.png",
        "Popcorn_Ceiling_1.1.png",
        "RoughstoneFloor_Floor_1.0.png",
        "SpecksofdustFloor_Floor_1.0.png",
        "StoneFloor_Floor_1.0.png",
        "WhitespottedFloor_Floor_1.0.png",
        "Blue_Roof_1.1.png",
        "BrownWood_Prop_1.1.png",
        "Conductor_Roof_1.1.png",
        "DarkerLeather_Prop_1.0.png",
        "GrayPlaster_Wall_1.0.png",
        "IronSand_Wall_1.0.png",
        "MetalMesh_Prop_1.0.png",
        "Plainwhite_Ceiling_1.1.png",
        "RedPlaster_Wall_1.0.png",
        "Rubber_Prop_1.0.png",
        "Spruce_Roof_1.1.png",
        "Tan_Ceiling_1.1.png",
        "Wood_Ceiling_1.1.png",
        "YellowMetal_Prop_1.0.png",
        "BrownLeather_Prop_1.0.png",
        "Cement_Ceiling_1.1.png",
        "CubepatternFloor_Floor_1.1.png",
        "Emerald_Roof_1.1.png",
        "GreenPlaster_Wall_1.0.png",
        "Knockdown_Ceiling_1.1.png",
        "MetalPatterned_Roof_1.1.png",
        "Plank_Ceiling_1.1.png",
        "Rough_Wall_1.0.png",
        "SmoothStoneFloor_Floor_1.1.png",
        "SteelMesh_Prop_1.0.png",
        "TriangleTile_Roof_1.1.png",
        "Wood_Wall_1.0.png"
    };
    */

    /*
    for (int i = 0; i < 50; ++i) {
        string filepath = "D:/AAASchool/4th Year/Term 1/IETECH/Assets/OriginalTextures/" + to_string(i) + ".png";
        Mat texture = imread(filepath);
        if (!texture.empty()) {
            baseTextures.push_back(texture);
        }
        else {
            cerr << "Failed to load texture: " << filepath << endl;
        }
    }
    */

    /*
    for (const auto& name : textureNames) {
        string filepath = "D:/AAASchool/4th Year/Term 1/IETECH/Assets/OriginalTextures/" + name + ".png";
        Mat texture = imread(filepath);
        if (!texture.empty()) {
            baseTextures.push_back(texture);
        }
        else {
            std::cerr << "Failed to load texture: " << filepath << std::endl;
        }
    }
    */

    string textureName = "AcaciaSaloon_Roof_1.1";
    string filepath = "D:/AAASchool/4th Year/Term 1/IETECH/Assets/OriginalTextures/AcaciaSaloon_Roof_1.1.png";
    Mat texture = imread(filepath);
    if (!texture.empty()) {
        baseTextures.push_back(texture);
    }
    else {
        cerr << "Failed to load texture: " << filepath << endl;
    }

    // Directory to save generated textures
    string outputDir = "D:/AAASchool/4th Year/Term 1/IETECH/Week3/GeneratedTextures/";

    // Generate 25 variations per texture
    int variationCount = 25;
    for (int i = 0; i < baseTextures.size(); ++i) {
        generateVariations(baseTextures[i], baseTextures, i, variationCount, textureName, outputDir);
    }

    cout << "Generated 1225 textures successfully!" << endl;
    return 0;
}


