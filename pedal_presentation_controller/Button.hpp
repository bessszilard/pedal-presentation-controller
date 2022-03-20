class Button {
public:
    Button();

    void configure(int p_pin, int p_logic);

    bool isPressed();

    bool currentState();

private:
    int m_pin;
    int m_logic;
    int m_prevState;
};