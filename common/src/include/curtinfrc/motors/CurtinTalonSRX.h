#pragma once

#include <SpeedController.h>
#include <ctre/phoenix/MotorControl/CAN/TalonSRX.h>

namespace curtinfrc {

	class CurtinTalonSRX : public frc::SpeedController,
		public ctre::phoenix::motorcontrol::can::TalonSRX {

		typedef ctre::phoenix::motorcontrol::ControlMode ControlMode;

		void SetControlMode(ControlMode mode) {
			_ctrl_mode = mode;
		}

		void Set(double speed) override {
			TalonSRX::Set(_ctrl_mode, speed);
			_val = speed;
		}
		
		double Get() const override {
			return _val;
		}

		void SetInverted(bool inv) override {
			TalonSRX::SetInverted(inv);
		}

		bool GetInverted() const override {
			return TalonSRX::GetInverted();
		}

		void Disable() override {
			TalonSRX::NeutralOutput();
		}

		void StopMotor() override {
			TalonSRX::NeutralOutput();
		}

	private:
		ControlMode _ctrl_mode = ControlMode::PercentOutput;
		double _val;
	};

} // ns curtinfrc