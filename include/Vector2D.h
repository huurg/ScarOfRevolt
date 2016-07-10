#ifndef VECTOR2D
#define VECTOR2D
class Vector2D {
public:
    double r[2];

    Vector2D();
    Vector2D(double inX, double inY);
    Vector2D(const Vector2D& inV);
    double x() const;
    double y() const;
    void x(double);
    void y(double);
    double dot(const Vector2D& inV) const;
    Vector2D times(const double inA) const;
    double R() const;
    double R2() const;
    Vector2D operator*(const double inA) const;
    double operator*(const Vector2D& inV) const;
    Vector2D operator+(const Vector2D& inV) const;
    Vector2D operator-(const Vector2D& inV) const;
    void operator=(const Vector2D& inV);

    void render(double inR, double inG, double inB) const;
};
#endif // VECTOR2D
