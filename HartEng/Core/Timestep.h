#pragma once

namespace HE
{
    class Timestep
    {
    private:
        double m_Time;
    public:
        Timestep(double time = 0.0f):
            m_Time(time)
        {

        }

        operator double() const
        {
            return m_Time;
        }

        void SetTime(double time)
        {
            m_Time = time;
        }

        inline double GetSeconds() const
        {
            return m_Time;
        }
        double GetMilliseconds() const
        {
            return m_Time * 1000.0f;
        }

    };

}
