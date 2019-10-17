#include "TX_Functions.h"

void setState(bool isTrainLightOn, bool isboomGateClosed)
{
	//pthread_rwlock_wrlock(&GPIO_Lock);

	if(crossingMode.crossing == automatic) crossing.trainLight = isTrainLightOn;
    if(crossingMode.crossing == automatic) crossing.boomGate = isboomGateClosed;

	//pthread_rwlock_unlock(&GPIO_Lock);
}

/*void blockSouth(void)
{
	//pthread_rwlock_wrlock(GPIO_Lock);

	intersectionMode.west.right = manual;
	intersection.west.right = red;

	intersectionMode.north.straight = manual;
	intersection.north.straight = red;

	intersectionMode.east.left = manual;
	intersection.east.left = red;

	//pthread_rwlock_unlock(GPIO_Lock);
};

void blockNorth(void)
{
	//pthread_rwlock_wrlock(GPIO_Lock);

	intersectionMode.east.right = manual;
	intersection.east.right = red;

	intersectionMode.south.straight = manual;
	intersection.south.straight = red;

	intersectionMode.west.left = manual;
	intersection.west.left = red;

	//pthread_rwlock_unlock(GPIO_Lock);
};

void unblockSouth(void)
{
	//pthread_rwlock_wrlock(GPIO_Lock);

	intersectionMode.west.right = automatic;
	intersectionMode.north.straight = automatic;
	intersectionMode.east.left = automatic;

	//pthread_rwlock_unlock(GPIO_Lock);
};

void unblockNorth(void)
{
	//pthread_rwlock_wrlock(GPIO_Lock);

	intersectionMode.east.right = automatic;
	intersectionMode.south.straight = automatic;
	intersectionMode.west.left = automatic;

	//pthread_rwlock_unlock(GPIO_Lock);
};

void setLight(char buffer[256]){
	int *ptr;
	ptr = &intersection.north.pedestrian1;

	int *ptrMode;
	ptrMode = &intersectionMode.north.pedestrian1;

	switch((int)buffer[0]){
		case N:
			ptr += 0;
			ptrMode += 0;
			break;
		case S:
			ptr += 5;
			ptrMode += 5;
			break;
		case E:
			ptr += 10;
			ptrMode += 10;
			break;
		case W:
			ptr += 15;
			ptrMode += 15;
			break;
		default:
			printf("direction not defined");
			break;
	}

	switch((int)buffer[1]){
		case P:
			ptr += 0;
			ptrMode += 0;
			break;
		case L:
			ptr += 1;
			ptrMode += 1;
			break;
		case S:
			ptr += 2;
			ptrMode += 2;
			break;
		case R:
			ptr += 3;
			ptrMode += 3;
			break;
		default:
			printf("light not defined");
			break;
	}

	//pthread_rwlock_wrlock(GPIO_Lock);
	switch((int)buffer[2]){
		case R:
			if((int)buffer[1] == P){
				*ptrMode = manual;
				*ptr = red;
				ptr += 4;

				ptrMode += 4;
				*ptrMode = manual;
				*ptr = red;
			} else {
				*ptrMode = manual;
				*ptr = red;
			}
			break;
		case Y:
			if((int)buffer[1] == P){
				*ptrMode = manual;
				*ptr = yellow;
				ptr += 4;
				ptrMode += 4;
				*ptrMode = manual;
				*ptr = yellow;
			} else {
				*ptrMode = manual;
				*ptr = yellow;
			}
			break;
		case G:
			if((int)buffer[1] == P){
				*ptrMode = manual;
				*ptr = green;
				ptr += 4;
				ptrMode += 4;
				*ptrMode = manual;
				*ptr = green;
			} else {
				*ptrMode = manual;
				*ptr = green;
			}
			break;
		case O:
			if((int)buffer[1] == P){
				*ptrMode = manual;
				*ptr = off;
				ptr += 4;
				ptrMode += 4;
				*ptrMode = manual;
				*ptr = off;
			} else {
				*ptrMode = manual;
				*ptr = off;
			}
			break;
		case A:
			if((int)buffer[1] == P){
				*ptrMode = automatic;
				ptrMode += 4;
				*ptrMode = automatic;
			} else {
				*ptrMode = automatic;
			}
			break;
		default:
			printf("value not defined");
			break;
	}
	//pthread_rwlock_unlock(GPIO_Lock);

}*/

