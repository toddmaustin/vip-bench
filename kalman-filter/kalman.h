/**
 kalman.h
 
 A very simple and versatile Kalman filter

 @author Tobias Ebsen
 @version 1.0.0 23/4/2018

 MIT License

 Copyright (c) 2020 Tobias Ebsen

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */

#ifndef kalman_h
#define kalman_h


// 2D Vector
class Vec2 {
public:
    float x;
    float y;

    Vec2();
    Vec2(float x, float y);

    Vec2 operator * (const float & scalar) const;
    Vec2 operator * (const Vec2 & other) const;
    Vec2 operator + (const Vec2 & other) const;
    float & operator [](int index);
};


// 2x2 Matrix
class Mat2x2 {
public:
    float m00;
    float m01;
    float m10;
    float m11;

    Mat2x2();
    Mat2x2(float m00, float m01, float m10, float m11);

    Mat2x2 transposed();
    Mat2x2 inversed();

    Mat2x2 operator * (const Mat2x2 & other) const;
    Vec2   operator * (const Vec2 & other) const;
    Mat2x2 operator * (const float & scalar) const;
    Mat2x2 operator + (const Vec2 & other) const;
    Mat2x2 operator + (const float & scalar) const;
    Mat2x2 operator - (const Mat2x2 & other) const;
};


// Kalman Model class
class KalmanModel {
public:
    KalmanModel();
    
    void predict(float dt, float u, Vec2 Q);
    void correct(float z, float R);

    void reset();

    // State
    Vec2 x;

    // Error covariance
    Mat2x2 P;
};


// Kalman Filter class
class KalmanFilter {
public:
    KalmanFilter();
    KalmanFilter(unsigned int nmodels);
    ~KalmanFilter();

    void init(unsigned int nmodels = 1) {this->nmodels = nmodels; pmodels = new KalmanModel[nmodels];}

    void setProcessNoise(float Qpos, float Qvel) {Q.x = Qpos;Q.y = Qvel;}
    void setMeasurementNoise(float R) {this->R = R;}

    // Predict state from current error covariance (single state).
    void predict(float dt);
    // Predict multiple states from current error covariance.
    void predict(float dt, float * pu, unsigned int nu, unsigned int start = 0);

    // Correct prediction with observation (single state).
    void correct(float z, unsigned int istate = 0);
    // Correct multiple predictions with observations.
    void correct(float * pz, unsigned int nz, unsigned int start = 0);

    // Get current states
    float get(unsigned int istate = 0);
    void get(float * px, unsigned int nx, unsigned int start = 0);

    // Set current state
    void set(float z, unsigned int istate = 0);
    void set(float * px, unsigned int nx, unsigned int start = 0);

    // Reset state
    void reset();
    void reset(unsigned int n, unsigned int start = 0);

private:

    // Process noise
    Vec2 Q;

    // Measurement noise
    float R;

    KalmanModel * pmodels;
    unsigned int nmodels;
};


#endif /* kalman_h */
