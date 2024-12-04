#include "./brightener.h"

ImageBrightener::ImageBrightener(std::shared_ptr<Image> inputImage): m_inputImage(inputImage) {
}
void ImageBrightener::brightenPixel(int x, int y, int& attenuatedPixelCount) {
    int pixelIndex = m_inputImage->getPixelIndex(x, y);
    if (m_inputImage->pixels[pixelIndex] > (255 - 25)) {
        ++attenuatedPixelCount;
        m_inputImage->pixels[pixelIndex] = 255;
    } else {
        m_inputImage->pixels[pixelIndex] += 25;
    }
}

int ImageBrightener::BrightenWholeImage() {
    // For brightening, we add a certain grayscale (25) to every pixel.
    // While brightening, some pixels may cross the max brightness. They are
    // called 'attenuated' pixels
    m_inputImage->processPixels([&attenuatedPixelCount, this](int x, int y) {
        brightenPixel(x, y, attenuatedPixelCount);
    });
    return attenuatedPixelCount;
}
void ImageBrightener::addBrighteningPixel(int x, int y, int& attenuatedCount, std::shared_ptr<Image> imageToAdd) {
    int pixelIndex = m_inputImage->getPixelIndex(x, y);
    if (static_cast<int>(m_inputImage->pixels[pixelIndex]) + imageToAdd->pixels[pixelIndex] > 255) {
        ++attenuatedCount;
        m_inputImage->pixels[pixelIndex] = 255;
    } else {
        m_inputImage->pixels[pixelIndex] += imageToAdd->pixels[pixelIndex];
    }
}
bool ImageBrightener::AddBrighteningImage(std::shared_ptr<Image> imageToAdd, int& attenuatedCount) {
    if (imageToAdd->m_rows != m_inputImage->m_rows || imageToAdd->m_columns != m_inputImage->m_columns) {
        return false;
    }
    attenuatedCount = 0;
    m_inputImage->processPixels([&attenuatedCount, this, imageToAdd](int x, int y) {
        addBrighteningPixel(x, y, attenuatedCount, imageToAdd);
    });
    return true;
}
