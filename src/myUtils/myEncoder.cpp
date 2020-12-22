// myEncoder.cpp
#include "myUtils/myEncoder.h"

void MyEncoder::Init(
    const dsy_gpio_pin pinA, 
    const dsy_gpio_pin pinB, 
    const dsy_gpio_pin pin_click, 
    const float update_rate, 
    const int16_t min_val, 
    const int16_t max_val)
{
    pinA_.pin = pinA;
    pinA_.mode = DSY_GPIO_MODE_INPUT;
    pinA_.pull = DSY_GPIO_PULLUP;
    dsy_gpio_init(&pinA_);

    pinB_.pin = pinB;
    pinB_.mode = DSY_GPIO_MODE_INPUT;
    pinB_.pull = DSY_GPIO_PULLUP;
    dsy_gpio_init(&pinB_);

    val_ = 0;
    inc_ = 0;

    min_val_ = min_val;
    max_val_ = max_val;

    value_on_rising_ = false;
    value_on_falling_ = true;
    reset_on_pressed_ = false;

    transitions_ = 0;
    prev_state_ = 0;

    wrap_mode_ = true;

    encoder_linked_led_ = NULL;

    MySwitch::Init(pin_click, update_rate, true);
}

void MyEncoder::SetRange(
    const int16_t min_val, 
    const int16_t max_val)
{
    min_val_ = min_val;
    max_val_ = max_val;   
}

void MyEncoder::Debounce()
{
    // Call the debounce for the switch
    MySwitch::Debounce();

    // Reset the encoder value if the switch has been pressed
    if (reset_on_pressed_ && Pressed())
    {
        Reset();
    }

    // Code for a rotary encoder, based on looking for transitions of the form:
    // 00 -> 01 -> 11 or 11 -> 10 -> 00 for CW rotation
    // 00 -> 10 -> 11 or 11 -> 01 -> 00 for CCW rotation
    // Fast, reliable, and doesn't require any debouncing of the state changes.

    // Note: assumes standard C++ conversion of true to 1 and false to 0

    uint8_t cur_state = (dsy_gpio_read(&pinA_) ? 1 : 0) + 2 * (dsy_gpio_read(&pinB_) ? 1 : 0);

    // If we've got a change of state, record it in the transitions
    if (cur_state != prev_state_) {
        // Add the new state change to the transitions, only keeping the 6 least significant bits
        transitions_ = ((transitions_ << 2) & 0b111111) | cur_state;

        inc_ = 0;

        // Check if the transitions match the patterns required
        // For the encoders we're using with the Super Pod and Petal it goes through
        // a full cycle in both A and B for each detent, and appears to finish with
        // both A and B high. We only look for transitions 0b000111 and 0b001011,
        // and ignore 0b111000 and 0b110100
        if ((value_on_rising_ && transitions_ == 0b000111) || (value_on_falling_ && transitions_ == 0b111000))  // CW direction
        {
            inc_ = 1;
            val_++;
            if (val_ > max_val_)
            {
                val_ = wrap_mode_ ? min_val_ : max_val_;
            }
        }
        else if ((value_on_rising_ && transitions_ == 0b001011) || (value_on_falling_ && transitions_ == 0b110100)) // CCW direction
        {
            inc_ = -1;
            val_--;
            if (val_ < min_val_)
            {
                val_ = wrap_mode_ ? max_val_ : min_val_;
            }
        }

        prev_state_ = cur_state;
    }

    if (encoder_linked_led_ != NULL && inc_ != 0)
	{
		encoder_linked_led_->SetValue(val_);
	}
}

void MyEncoder::SetValue(const int16_t val)
{
    val_ = val;

    if (encoder_linked_led_ != NULL)
	{
		encoder_linked_led_->SetValue(val_);
	}
}

void MyEncoder::Reset()
{
    SetValue(0);
    MySwitch::Reset();
}

void MyEncoder::SetLinkLed( MyRgbLed *linked_led )
{
    encoder_linked_led_ = linked_led;

    if (encoder_linked_led_ != NULL)
	{
		encoder_linked_led_->SetValue(val_);
	}
}
