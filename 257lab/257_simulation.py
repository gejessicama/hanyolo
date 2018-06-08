"""
    Simulation of heat wave from specified constants.
    Run from main
    Will plot graph of T vs X iteratively over time
    Wait for time threshold to be reached or force stop python to stop simulation
"""

import numpy as np
import matplotlib.pyplot as plt
import math

def HeatEquation_w_convection_radiation(show_sim,sensor_sim,total_t):
    """
        Function plots graphs of the heat equation simulation

    :param show_sim: (bool) show simulation of rod T wrt x over time if true
    :param sensor_sim: (bool) show T at each sensor wrt time
    :param total_t: total time for simulation run
    :return:
    """
    # Defining constants
    l, r = .3, 0.01 # length (m), radius (m) of rod
    dx, dt = .01, .5 #distance step, time step for simulation
    N = int(l / dx)
    k, c, p = 205.0, 900.0, 2700.0 #Thermal conductivity, specific heat,
    kc,eps,sig = 5,1,5.67*10**-8 #Convection coefficient, Stefan-Boltzman constant, emissivity

    Pin_0 = 12
    Pin = Pin_0 #Resistor output power
    T_amb = 20.0 + 273.15 #Ammbient temperature
    cons = (k / (c * p)) #Thermal difussivity
    P_dT_conv = c * p * math.pi* r ** 2 * dx #coefficient for converting power to temperature

    # Set initial temperatures for the rod
    T = [T_amb for x in range(N)]
    x = [i * dx for i in range(N)]
    s1,s2,s3,s4,s5 = 0.01,0.08,0.15,0.22,0.29#location of temperature sensors: 1.0,8.2,15.2,22.5,29.4 - reverse numbering to physical experiment
    is1,is2,is3,is4,is5 = 0,0,0,0,0 # indices of sensors in x
    for i in range(len(x)):
        if x[i] == s1:
            is1 = i
        if x[i] == s2:
            is2 = i
        if x[i] == s3:
            is3 = i
        if x[i] == s4:
            is4 = i
        if x[i] == s5:
            is5 = i
    #print 'indices ', is1,is2,is3,is4,is5
    """
    T_0,T_ = 20,0
    kw,w = 10.5,0.0185
    #T_x_t = lambda x_,t_: T_0*math.exp(-kw*x)*math.cos((w*t_-kw*x))+T_
    #T_arr = []
    """

    T_t = []
    # Run model of heat flow for time steps until threshold time limit is reached
    t = 0.0
    thresh = total_t #time (s)
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
            if (Pin == Pin_0):
                Pin = 0
            else:
                Pin = Pin_0
        T_sens = [T[is1],T[is2],T[is3],T[is4],T[is5]]
        T_t.append(T_sens)
        #display plot after every 10 data points sampled to reduce time delay
        if(t%(dt*10) == 0 and show_sim):
            plt.clf()
            plt.scatter(x, T)
            plt.title("Temperature versus position on the rod")
            plt.ylabel("Temperature [K]")
            plt.xlabel("Position on rod [m]")
            plt.title('T[0] = %d,T[N] = %d, t = %d'%(T[0],T[N-1],t))
            plt.pause(0.00001)

    if sensor_sim:
        t_arr = np.linspace(0,thresh,thresh/dt)
        T_t = np.transpose(np.vstack(T_t))

        plt.close()
        plt.plot(t_arr,T_t[:][0],label='s1')
        plt.plot(t_arr,T_t[:][1],label='s2')
        plt.plot(t_arr,T_t[:][2],label='s3')
        plt.plot(t_arr,T_t[:][3],label='s4')
        plt.plot(t_arr,T_t[:][4],label='s5')
        plt.legend(loc='lower right')
        plt.title('Temp vs time')
        plt.xlabel('time[s]')
        plt.ylabel('Temp [C]')
        plt.show()

if __name__ == "__main__":
    HeatEquation_w_convection_radiation(False,True,10000)