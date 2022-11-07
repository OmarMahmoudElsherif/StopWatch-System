/*
 ============================================================================
 Name        : mini_proj1.c
 Author      : Omar Mahmoud Mohamed Khalil Elsherif   , FACEBOOK NAME : Omar Elsherif
 Date        : 22/8/2022
 Copyright   : Your copyright notice
 Description : Mini-project1: Vehicle Control system
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

#define ON 1
#define OFF 0


#define WITH_ENGINE_TEMP_CONTROLLER 1



typedef struct {                   // structure for vehicle , has its temp,speed,Ac...etc
	unsigned char vehicleSpeed;
	unsigned char roomTemp;
	unsigned char engineState;
	unsigned char AC;

#if WITH_ENGINE_TEMP_CONTROLLER
	unsigned char engineTemp;
	unsigned char engineTempController;
#endif

}Vehicle;




void DisplaySpecifications();
void TurnEngineON();
void TurnEngineOFF();
void QuitProgram();


void DisplayVehicleContents(const Vehicle*);
void SetTrafficLightColor(Vehicle*);
void SetRoomTemperature(Vehicle*);
void SetEngineTemperature(Vehicle*);



int main(void) {

	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);

	while(1)
	{
		char selection;
		DisplaySpecifications();         //Display Menu
		void (*program[3])()={TurnEngineON,TurnEngineOFF,QuitProgram};
		scanf(" %c",&selection);
		selection-=97;          //To make a=0, b=1, c=2

		(*program[selection])();


		Vehicle car;     //making object of struct called vehicle

		/* initial values for vehicle */
		car.vehicleSpeed=60;              //60- initial value

#if WITH_ENGINE_TEMP_CONTROLLER
		car.engineTemp=90;                //90- initial value
		car.engineTempController=OFF;     //-->assume engTempCont is OFF initially
#endif
		car.roomTemp=35;                  //35- initial value
		car.engineState=ON;               //-->assume engine is running initially
		car.AC=OFF;                       // -->assume AC is OFF initially





		/* while the engine is ON ,keep looping */

		while(selection == 0)
		{
			scanf(" %c",&selection);
			selection-=97;            // to make a=0, b=1, c=2, d=3

			if(selection==0)          // engine off, a. Turn off the engine
			{
				(*program[1])();       //Turn engine OFF
				break;
			}
			else if(selection==1)           // b. Set the traffic light color.
				SetTrafficLightColor(&car);

			else if(selection==2)           //c. Set the room temperature (Temperature Sensor)
				SetRoomTemperature(&car);

#if WITH_ENGINE_TEMP_CONTROLLER
			else if(selection==3)           //d. Set the engine temperature (Engine Temperature Sensor)
				SetEngineTemperature(&car);
#endif

			DisplayVehicleContents(&car);   //Display the current vehicle state

			(*program[0])();       //displaying Sensor menu
			selection=0;           //to continue in mood Engine ON
		}

	}

	return 0;
}





void DisplaySpecifications()
{
	printf("a. Turn ON the vehicle engine\n");
	printf("b. Turn OFF the vehicle engine\n");
	printf("c. Quit the system\n\n");
}

void TurnEngineOFF()
{
	printf("Turn OFF Vehicle engine\n\n");
}


void TurnEngineON()
{
	printf("---------Sensor Set Menu---------\n");
	printf("a. Turn off the engine\n");
	printf("b. Set the traffic light color \n");
	printf("c. Set the room temperature(Temperature Sensor)\n");

#if WITH_ENGINE_TEMP_CONTROLLER
	printf("d. Set the engine temperature\n\n");
#endif
}

void QuitProgram()
{
	printf("Quit the System\n");
	exit(0);        // Terminate the program
}


void DisplayVehicleContents(const Vehicle* veh)
{

	veh->vehicleSpeed>0 ? printf("\n\nEngine state: ON\n"):printf("\n\nEngine state: OFF\n");
	veh->AC ? printf("AC: ON\n"):printf("AC: OFF\n");
	printf("Vehicle Speed: %u Km/hr \n",veh->vehicleSpeed);
	printf("Room Temperature: %u C\n",veh->roomTemp);

#if WITH_ENGINE_TEMP_CONTROLLER
	veh->engineTempController ? printf("Engine Temperature Controller State: ON\n")
			:printf("Engine Temperature Controller State: OFF\n");
	printf("Engine Temperature: %u C\n\n",veh->engineTemp);
#endif
}


void SetTrafficLightColor(Vehicle* veh)
{
	char color;
	printf("Enter the required color {'G','O','R'} : ");
	scanf(" %c",&color);
	if(color=='G'|| color== 'g')
	{
		veh->vehicleSpeed=100;
	}
	else if(color=='O' || color=='o')
	{
		veh->vehicleSpeed=30;

		/* if speed=30, turn AC on and set roomtemp and engine temp to
		  current temp*(5/4)+1
		 */
		veh->AC=ON;
		veh->roomTemp=(veh->roomTemp)*(5/4) +1;

#if WITH_ENGINE_TEMP_CONTROLLER
		veh->engineTempController=ON;
		veh->engineTemp=(veh->engineTemp)*(5/4) +1;
#endif
	}
	else if(color=='R' || color=='r')
	{
		veh->vehicleSpeed=0;
		veh->engineState=OFF; 	  //Engine is OFF
	}
	else
	{
		printf("Invalid Color , please Enter Color Again...\n");
		SetTrafficLightColor(veh);
	}

}


void SetRoomTemperature(Vehicle*veh)
{
	int roomtemp;
	printf("Enter the required room temperature: ");
	scanf(" %d",&roomtemp);
	if(roomtemp>=30 || roomtemp<10)
	{
		veh->AC=ON;
		veh->roomTemp=20;
	}
	else
	{
		veh->roomTemp=roomtemp;
		veh->AC=OFF;
	}
}

#if WITH_ENGINE_TEMP_CONTROLLER
void SetEngineTemperature(Vehicle* veh)
{
	int enginetemp;
	printf("Enter the required engine temperature: ");
	scanf("%d",&enginetemp);
	if(enginetemp>=150 || enginetemp<=100)
	{
		veh->engineTempController=ON;
		veh->engineTemp=125;
	}
	else
	{
		veh->engineTemp=enginetemp;
		veh->engineTempController=OFF;
	}

}
#endif
