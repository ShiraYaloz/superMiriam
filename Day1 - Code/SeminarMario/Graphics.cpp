#include "Graphics.h"
#include <algorithm>

using namespace cv;
using namespace std;

#define LIVES_OFFSET 50
//////////////////////////////////////////////////

SingleAnimationGraphics::SingleAnimationGraphics(AnimationPtr animPtr, bool isCyclic)
    :_animation(animPtr), _currFrameIdx(0)
    , _isCyclic(isCyclic)
{    
}


cv::Mat SingleAnimationGraphics::getCollisionMask()
{
    return _animation->getFrame(_currFrameIdx).mask;
}

void SingleAnimationGraphics::draw(cv::Mat& canvas, cv::Point const& topLeft)
{
    drawFrame(_animation->getFrame(_currFrameIdx), canvas, topLeft);
}

void SingleAnimationGraphics::reset(int code)
{
    if (code != -1)
          _currFrameIdx = 0;
}

bool SingleAnimationGraphics::update()
{
    bool hasFinished = (_currFrameIdx == _animation->numFrames() - 1) || _isCyclic;
    if (_isCyclic)
        // למה לא פשוט 0
        _currFrameIdx = (_currFrameIdx + 1) % _animation->numFrames();
    else 
        _currFrameIdx = min<int>(_currFrameIdx + 1, (int)_animation->numFrames() - 1);

    return hasFinished;
}


/////////////////////////---------------put text----------------------/////////////////////////

void exampleOfPutText()
{
    Mat image = imread(R"(../Animations/background.png)", IMREAD_UNCHANGED);
    string text = "score: ";
    int score = 465;
    string moreText = to_string(score);
    text = text + moreText;
    Point topLeft(50, 50);
    int fontFace = FONT_HERSHEY_SIMPLEX; // normal size sans - serif font
    // other options for fontFace:
    FONT_HERSHEY_PLAIN;// small size sans - serif font
    FONT_HERSHEY_DUPLEX;// normal size sans - serif font(more complex than FONT_HERSHEY_SIMPLEX)
    FONT_HERSHEY_COMPLEX; // normal size serif font
    FONT_HERSHEY_TRIPLEX; //normal size serif font(more complex than FONT_HERSHEY_COMPLEX)
    FONT_HERSHEY_COMPLEX_SMALL; //smaller version of FONT_HERSHEY_COMPLEX
    FONT_HERSHEY_SCRIPT_SIMPLEX; //hand - writing style font
    FONT_HERSHEY_SCRIPT_COMPLEX;// more complex variant of FONT_HERSHEY_SCRIPT_SIMPLEX
    float fontScale = 2; // Font scale factor that is multiplied by the font-specific base size.
    Scalar color(/*Blue between 0...255 =*/0,/*Green between 0...255 =*/0,/*Red between 0...255 =*/255); 
    int thickness = 2;
    int lineType = FILLED;
    // other options for line type:
    LINE_4; LINE_8; LINE_AA;

    // finally:
    putText(image, text, topLeft, fontFace, fontScale, color, thickness, lineType);
    
    imshow("test", image);
    waitKey();//waiting forever till some key is hit.
}
/////////////////////////---------//////////////////////////////////////////////

LivesGraphics::LivesGraphics(std::string const& path, int lCount)
    : _singleLife(path), _livesCount(lCount)
{
}

cv::Mat LivesGraphics::getCollisionMask()
{
    return _singleLife.mask;
}

void LivesGraphics::draw(cv::Mat& canvas, cv::Point const& topLeft)
{
    Point withOffset = topLeft;
    for (size_t i = 0; i < _livesCount; i++)
    {
    drawFrame(_singleLife, canvas, withOffset);
    withOffset.x += LIVES_OFFSET;
    }
    
}

void LivesGraphics::reset(int code)
{
    if (code == -1) {
        _livesCount += 1;
    }
    else
    _livesCount = code;
}

bool LivesGraphics::update()
{
    // מתי שהפיזיקה של הלבבות נגמרה המשחק נגמר
    _livesCount--;  
    if (_livesCount < 0)
        _livesCount = 0;
    if (_livesCount == 0)
        return true;
 
    return false;
}
///////////////////////////-----------/////////////////////////////

ScoresGraphics::ScoresGraphics(float fScale, int initialScore, int fontFace)
    :_fontScale(fScale), _score(initialScore), _fontFace(fontFace)
{}

cv::Mat ScoresGraphics::getCollisionMask()
{
    return cv::Mat();
}

void ScoresGraphics::draw(cv::Mat& canvas, cv::Point const& topLeft)
{
    //Mat image = imread(R"(../Animations/background.png)", IMREAD_UNCHANGED);
    string text = "score: " + to_string(_score);
    Scalar color(0,0,255);
    int thickness = 2;
    int lineType = FILLED;
    // finally:
    putText(canvas, text, topLeft, _fontFace, _fontScale, color, thickness, lineType);
    imshow("test", canvas);
}

void ScoresGraphics::reset(int code)
{
    if (code == -1) {
        _score += 200;
    }
    else
    _score = code;
}

bool ScoresGraphics::update()
{
    _score -= 100;
    return false;
}
////////////////////////////////// --- Apple-----/////////////////////
AppleGraphics::AppleGraphics(std::string const& path)
    :_applePic(path)
{
}

cv::Mat AppleGraphics::getCollisionMask()
{
    return _applePic.mask;
}

void AppleGraphics::draw(cv::Mat& canvas, cv::Point const& topLeft)
{
    drawFrame(_applePic, canvas, topLeft);
}

void AppleGraphics::reset(int code)
{
}

bool AppleGraphics::update()
{
    return false;
}
