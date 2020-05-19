/*
 * I2C.h
 *
 *  Created on: May 16, 2020
 *      Author: Reem
 */

#ifndef MCAL_I2C_H_
#define MCAL_I2C_H_
#include "I2C_LCfg.h"
#include "ERROR.h"


/*
 * Function Name: _I2C_Init
 * Input:
 * Output:
 * Return: ERROR Status
 * Description: This Function used for initializing the configured struct in I2C_LCfg
 * */
extern ERROR_STATUS _I2C_Init(void);

/*
 * Function Name: _I2C_SendDataMaster
 * Input: - Data         -> Pointer to Data bytes to be written by Master
 *        - DataSize     -> Length of the bytes to be written
 *        - I2Cx         -> I2C Channel Number
 *        - SlaveAddress -> The Slave address the master will deal with
 * Output:
 * Return: ERROR Status
 * Description: This Function used for Master to send data
 * */
extern ERROR_STATUS _I2C_SendDataMaster(uint8_t* Data,uint16_t DataSize,uint8_t I2Cx,uint8_t SlaveAddress);

/*
 * Function Name: _I2C_SendDataSlave
 * Input: - Data         -> Pointer to Data bytes to be written by Slave
 *        - DataSize     -> Length of the bytes to be written
 *        - I2Cx         -> I2C Channel Number
 * Output:
 * Return: ERROR Status
 * Description: This Function used for Slave to send data
 * */
extern ERROR_STATUS _I2C_SendDataSlave(uint8_t* Data,uint16_t DataSize,uint8_t I2Cx);

/*
 * Function Name: _I2C_RecieveDataMaster
 * Input: - Data         -> Pointer to Data bytes to be read by Master
 *        - DataSize     -> Length of the bytes to be read
 *        - I2Cx         -> I2C Channel Number
 *        - SlaveAddress -> The Slave address the master will deal with
 * Output:
 * Return: ERROR Status
 * Description: This Function used for Master to receive data
 * */
extern ERROR_STATUS _I2C_RecieveDataMaster(uint8_t* Data,uint16_t DataSize,uint8_t I2Cx,uint8_t SlaveAddress);

/*
 * Function Name: _I2C_RecieveDataSlave
 * Input: - Data         -> Pointer to Data bytes to be read by Slave
 *        - DataSize     -> Length of the bytes to be read
 *        - I2Cx         -> I2C Channel Number
 * Output:
 * Return: ERROR Status
 * Description: This Function used for Slave to receive data
 * */
extern ERROR_STATUS _I2C_RecieveDataSlave(uint8_t* Data,uint16_t DataSize,uint8_t I2Cx,uint8_t SlaveAddress);
#endif /* MCAL_I2C_H_ */
