#ifndef PLAYER_INCLUDE
#define PLAYER_INCLUDE

class Player {
private:
    const double DEFAULT_CHARGE_MAX = 100.0;
    const double DEFAULT_CONCENTRATION_MAX = 100.0;
    const double DEFAULT_CHARGE_RATE = 0.5;
    const double DEFAULT_CONCENTRATION_RATE = 0.2;

    bool can_charge;
    double charge;
    double charge_rate;
    double charge_max;
    enum ChargeState {
        CHARGE_STATE_NONE,
        CHARGE_STATE_CHARGING
    };
    ChargeState chargeState;

    bool can_concentrate;
    double concentration;
    double concentration_rate;
    double concentration_max;
    enum ConcentrationState {
        CONCENTRATION_STATE_NONE,
        CONCENTRATION_STATE_CONCENTRATING
    };
    ConcentrationState concentrationState;

    bool up, down, held;

    void start_charge();
    void end_charge();
public:
    Player();

    void frameListen(bool inUp, bool inDown, bool inHeld);
    void step();
    void frameTell(double* inCharge, double* inConc);
};

#endif // PLAYER_INCLUDE
