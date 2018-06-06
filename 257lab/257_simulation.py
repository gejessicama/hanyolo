"""
    Simulation of heat wave from specified constants.
    Run from main
    Will plot graph of T vs X iteratively over time
    Wait for time threshold to be reached or force stop python to stop simulation
"""

import numpy as np
import matplotlib.pyplot as plt
import math

def HeatEquation_w_convection_radiation():
    # Defining constants
    l, r = .3, 0.01 # length (m), radius (m) of rod
    dx, dt = .01, .5 #distance step, time step for simulation
    N = int(l / dx)
    k, c, p = 205.0, 900.0, 2700.0 #Thermal conductivity, specific heat,
    kc,eps,sig = 5,1,5.67*10**-8 #Convection coefficient, Stefan-Boltzman constant, emissivity

    Pin = 15 #Resistor output power
    T_amb = 20.0 + 273.15 #Ammbient temperature
    cons = (k / (c * p)) #Thermal difussivity
    P_dT_conv = c * p * math.pi* r ** 2 * dx #coefficient for converting power to temperature

    # Set initial temperatures for the rod
    T = [T_amb for x in range(N)]
    x = [i * dx for i in range(N)]


    """
    T_0,T_ = 20,0
    kw,w = 10.5,0.0185
    #T_x_t = lambda x_,t_: T_0*math.exp(-kw*x)*math.cos((w*t_-kw*x))+T_
    #T_arr = []
    """


    # Run model of heat flow for time steps until threshold time limit is reached
    t = 0.0
    thresh = 10000 #time (s)
    Period = 340 #period (s) of turning Pin on/off
    plt.show()

    while (t<thresh):
        temp_arr = T[:]
        #determine change in temperature for inner sections of rod
        for l in xrange(1, N-1, 1):
            P_loss = 2.0 * math.pi * r * dx * kc*(temp_arr[l] - T_amb) + 2.0 * math.pi * r * dx * eps * sig*(
                temp_arr[l] ** 4 - T_amb ** 4)
            delT = (temp_arr[l - 1] - 2 * temp_arr[l] + temp_arr[l + 1]) * cons * (dt / (dx * dx))
            delT_conv_rad = -1*dt*P_loss / (P_dT_conv)
            T[l] += delT + delT_conv_rad

        #determine net power and change in temperature for end sections
        P_loss_0 = (2 * math.pi * r * dx + math.pi * r**2) *( kc*(temp_arr[0] - T_amb) +  eps * sig*(
            temp_arr[l] ** 4 - T_amb ** 4))
        P_loss_N = (2 * math.pi * r * dx + math.pi * r**2)* (kc*(temp_arr[N-1] - T_amb) + eps * sig*(
            temp_arr[l] ** 4 - T_amb ** 4))

        delP_0 = Pin-P_loss_0
        delT_conv_rad_0 = delP_0 *dt / (P_dT_conv)
        delT_conv_rad_N = -1*P_loss_N*dt / (P_dT_conv)


        T[0] = temp_arr[0] + (temp_arr[1] - temp_arr[0]) * cons * (dt / (dx * dx)) + delT_conv_rad_0
        T[N - 1] = temp_arr[N - 1] + (temp_arr[N - 2] - temp_arr[N - 1]) * cons * (dt / (dx * dx)) + delT_conv_rad_N


        # At each half cycle turn power on/off
        t +=dt
        if( t%(Period/2) == 0):
            print 'pin = ',Pin
            print 't = ', t
            if (Pin == 10):
                Pin = 0
            else:
                Pin = 10

        #display plot after every 10 data points sampled to reduce time delay
        if(t%(dt*10) == 0):
            plt.clf()
            plt.scatter(x, T)
            plt.title("Steady-state temperature versus position on the rod")
            plt.ylabel("Temperature (K)")
            plt.xlabel("Position on rod (m)")
            plt.title('T[0] = %d,T[N] = %d, t = %d'%(T[0],T[N-1],t))
            plt.pause(0.0001)

if __name__ == "__main__":
    HeatEquation_w_convection_radiation()
