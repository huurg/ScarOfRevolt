#ifndef CHARGE_BAR_UI_HEADER
#define CHARGE_BAR_UI_HEADER

class ChargeBarUI {
private:
    const double DEFAULT_HEIGHT = 0.5;
    const double DEFAULT_WIDTH = 6.0;
    const double DEFAULT_POSITION_X = 0.0;
    const double DEFAULT_POSITION_Y = 4.85;

    double height;
    double width;
    double position_x;
    double position_y;
    double percentage_charged;
    double charge;
    double max_charge;
    double percentage_conc;
    double conc;
    double max_conc;
public:
    ChargeBarUI(double inMaxCharge, double inMaxConc);

    void setCharge(double inCharge);
    void setConcentration(double inConc);
    void render();
};

#endif // CHARGE_BAR_UI_HEADER
