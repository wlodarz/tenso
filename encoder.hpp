#ifndef __ENCODER_HPP__
#define __ENCODER_HPP__

#include <phidget22.h>

class Encoder {
	public:
		Encoder();
		~Encoder();

		int init();

	private:
		PhidgetEncoderHandle m_encoder;
};

#endif // __ENCODER_HPP__

