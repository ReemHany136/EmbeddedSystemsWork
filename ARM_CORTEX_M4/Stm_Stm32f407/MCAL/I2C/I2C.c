/*
 * I2C.c
 *
 *  Created on: May 16, 2020
 *      Author: Reem
 */
#include "I2C.h"
#include "PerRegs.h"
#include "gpio.h"

/*CR1 MASKS To be Cleared*/
#define PERIPH_EN_MASK                  0X0001
#define ENPEC_MASK                      0X0020
#define ENGC_MASK                       0X0040
#define ENNO_STRETCING_MASK             0X0080
#define START_MASK                      0X0100
#define STOP_MASK                       0X0200
#define ACK_MASK                        0X0400
#define POS_MASK                        0X0800
#define PEC_MASK                        0X1000

/*CR2 MASKS To be Cleared*/
#define FREQ_MASK                       0X001F
#define ITERREN_MASK                    0X0100
#define ITEVENTEN_MASK                  0X0200
#define ITBUFEN_MASK                    0X0400

/*OAR1 MASKS To be Cleared*/
#define ADD0_MASK                       0X0001
#define SLAVEADD_7_1_MASK               0X00FE
#define ADD_9_8_MASK                    0X0300
#define ADDMODE_MASK                    0X8000

/*OAR2 MASKS To be Cleared*/
#define ENDUAL_MASK                     0X0001

/*DR MASKS To be Cleared*/
#define DR_MASK                         0X00FF

/*CCR MASKS To be Cleared*/
#define FSM_MASK                      0X8000
#define DUTY_MASK                     0X4000
#define CCR_MASK                      0X0FFF

/*TRISE MASKS To be Cleared*/
#define TRISE_MASK                      0X003F

/*FLTR MASKS To be Cleared*/
#define DNF_MASK                        0X000F
#define ANOFF_MASK                      0X0010

/*SR1 MASKS To be Cleared*/
#define SB_MASK                         0X0001
#define ADDR_MASK                       0X0002
#define BTF_MASK                        0X0004
#define ADD10_MASK                      0X0008
#define STOPF_MASK                      0X0010
#define RXNE_MASK                       0X0040
#define TXE_MASK                        0X0080
#define BERR_MASK                       0X0100
#define ARLO_MASK                       0X0200
#define AF_MASK                         0X0400
#define OVR_MASK                        0X0800
#define PECERR_MASK                     0X1000
#define TIMEOUT_MASK                    0X4000

/*SR2 MASKS To be Cleared*/
#define MSL_MASK                        0X0001
#define BUSY_MASK                       0X0002
#define TRA_MASK                        0X0004
#define GENCALL_MASK                    0X0010
#define DUALF_MASK                      0X0080

/*************************************************/
#define THIGH_DUTY0                     1
#define TLOW_DUTY0                      2
#define THIGH_DUTY1                     9
#define TLOW_DUTY1                      16
#define FM_MAX_CLK                      400
#define SM_MAX_CLK                      100

#define FREQ                            8
#define I2C1_CLK_MASK                   0X00200000
#define I2C2_CLK_MASK                   0X00400000
#define I2C3_CLK_MASK                   0X00800000

#define I2C_INT_DATA_BUFFER_SIZE        1000
#define LOCKED                          1
#define UNLOCKED                        0
#define READ_MODE                       1
#define WRITE_MODE                      0
#define MASTER                          0
#define SLAVE                           1

#define NUM_I2C_CHANNELS                3


/*Static Globals*/
static uint8_t  *gau8I2CBuffer[NUM_I2C_CHANNELS] ,gau8I2CBufferLocker[NUM_I2C_CHANNELS] ,
				 gau8I2CInitFlags[NUM_I2C_CHANNELS] = {FALSE, FALSE, FALSE};

static uint16_t gau16I2CBufferSize[NUM_I2C_CHANNELS] , gau16I2CBufferCounter[NUM_I2C_CHANNELS]
				, gau16I2CSlaveAddress[NUM_I2C_CHANNELS];

/*Static Prototypes*/

/*
 * Function Name: _I2C_SendData
 * Input: - Data         -> Pointer to Data bytes to be written by Master or Slave
 *        - DataSize     -> Length of the bytes to be written by Master or Slave
 *        - I2Cptr       -> Pointer to the selected I2C Channel
 * Output:
 * Return: ERROR Status
 * Description: This Function used for sending data using polling
 * */
static void _I2C_SendData(uint8_t * Data, uint16_t DataSize,str_I2C_t *I2Cptr);

/*
 * Function Name: _I2C_RecieveData
 * Input: - Data         -> Pointer to Data bytes to be read by Master or Slave
 *        - DataSize     -> Length of the bytes to be read by Master or Slave
 *        - I2Cptr       -> Pointer to the selected I2C Channel
 * Output:
 * Return: ERROR Status
 * Description: This Function used for receiving data using polling
 * */
static void _I2C_RecieveData(uint8_t * Data, uint16_t DataSize,str_I2C_t *I2Cptr);

/*
 * Function Name: _I2C_SendSB
 * Input: - I2Cptr -> Pointer to the selected I2C Channel
 * Output:
 * Return: ERROR Status
 * Description: This Function used for sending Start bit  by polling
 * */
static void _I2C_SendSB(str_I2C_t *I2Cptr);

/*
 * Function Name: _I2C_SendAddress
 * Input: - I2Cptr  -> Pointer to the selected I2C Channel
 * 		  - Address -> Slave address to be sent
 * Output:
 * Return: ERROR Status
 * Description: This Function used for sending Slave address by polling
 * */
static void _I2C_SendAddress(uint8_t Address,str_I2C_t *I2Cptr);

/*
 * Function Name: _I2C_SendSTOP
 * Input: - I2Cptr -> Pointer to the selected I2C Channel
 * Output:
 * Return: ERROR Status
 * Description: This Function used for sending Stop bit by polling
 * */
static void _I2C_SendSTOP(str_I2C_t *I2Cptr);

/*
 * Function Name: I2C_MasterSetter
 * Input: - Data         -> Pointer to Data bytes to be written or read by Master
 *        - DataSize     -> Length of the bytes to be written or read by Master
 *        - I2Cx         -> I2C Channel Number
 *        - SlaveAddress -> The Slave address the master will deal with
 * Output:
 * Return: ERROR Status
 * Description: This Function used for Saving the Input  Data from the Master to the shared Resources to be used in Interrupt
 * */
static void I2C_MasterSetter(uint8_t * Data,uint16_t DataSize, uint8_t I2Cx,uint16_t SlaveAddress);

/*
 * Function Name: I2C_SlaveSetter
 * Input: - Data         -> Pointer to Data bytes to be written or read by Slave
 *        - DataSize     -> Length of the bytes to be written or read by Slave
 *        - I2Cx         -> I2C Channel Number
 * Output:
 * Return: ERROR Status
 * Description: This Function used for Saving the Input  Data from the Slave to the shared Resources to be used in Interrupt
 * */
static void I2C_SlaveSetter(uint8_t * Data,uint16_t DataSize, uint8_t I2Cx);

/*Function Implementation*/

/*
 * Function Name: _I2C_Init
 * Input:
 * Output:
 * Return: ERROR Status
 * Description: This Function used for initializing the configured struct in I2C_LCfg
 * */
ERROR_STATUS _I2C_Init(void)
{
	ERROR_STATUS ERR = E_NOK;

	uint8_t i;
	str_I2C_t * I2C_used;
	uint8_t u8TRISE_, u8calculatedPrescale,u8CCR_Calc;
	//Initialize Globals
	for(i =0; i < NUM_I2C_CHANNELS; i++)
	{
		gau8I2CBufferLocker[i] =  UNLOCKED;
		gau16I2CBufferCounter[i] =  0;
		gau16I2CBufferSize[i] =  0;
	}

	for(i = 0;i < NUM_OF_I2C_OBJECTS;i++)
	{
		if(gau8I2CInitFlags[I2C_Cfg->I2C_ChannelNo] == TRUE)
		{
			//TODO: complete checking on all parameters
			//ERROR
			ERR = E_MULTIPLE_INITIALIZION;
			break;

		}
		else
		{
			if((I2C_Cfg[i].GeneralCallEN != GC_DISABLE && I2C_Cfg[i].GeneralCallEN != GC_ENABLE)||
				(I2C_Cfg[i].ClkStretchingEN != CLK_STRETCH_DISABLE && I2C_Cfg[i].ClkStretchingEN != CLK_STRETCH_ENABLE)||
				(I2C_Cfg[i].PEC_EN != PEC_DISABLE && I2C_Cfg[i].PEC_EN != PEC_ENABLE)||
				(I2C_Cfg[i].ACK_EN != ACK_DISABLE && I2C_Cfg[i].ACK_EN != ACK_ENABLE)||
				(I2C_Cfg[i].BufferInterruptEN != INT_BUFFER_DISABLE && I2C_Cfg[i].BufferInterruptEN != INT_BUFFER_ENABLE)||
				(I2C_Cfg[i].EventInterruptEN != INT_EVENT_DISABLE && I2C_Cfg[i].EventInterruptEN != INT_EVENT_ENABLE)||
				(I2C_Cfg[i].ErrorInterruptEN != INT_ERR_DISABLE && I2C_Cfg[i].ErrorInterruptEN != INT_ERR_ENABLE)||
				(I2C_Cfg[i].InterfaceAddMode != _7_BIT_ADDRESS && I2C_Cfg[i].InterfaceAddMode != _10_BIT_ADDRESS)||
				(I2C_Cfg[i].DualModeEN != DUAL_ADDRESS_ENABLE && I2C_Cfg[i].DualModeEN != DUAL_ADDRESS_DISABLE)||
				(I2C_Cfg[i].MasterModeSelection != FM_ENABLE && I2C_Cfg[i].MasterModeSelection != SM_ENABLE)||
				(I2C_Cfg[i].Duty != DUTY_2TLOW_1THIGH_ENABLE && I2C_Cfg[i].Duty != DUTY_16TLOW_9THIGH_ENABLE)||
				(I2C_Cfg[i].MasterModeSelection == FM_ENABLE && I2C_Cfg[i].CLK > FM_MAX_CLK) ||
				(I2C_Cfg[i].MasterModeSelection == SM_ENABLE && I2C_Cfg[i].CLK > SM_MAX_CLK))
			{
				ERR = E_INVALID_PARAMETER;

			}
			else
			{
				switch(I2C_Cfg[i].I2C_ChannelNo)
				{
				case I2C_1:
					I2C_used  = I2C1;
					RCC->APB1ENR &= ~(I2C1_CLK_MASK);
					RCC->APB1ENR |= I2C1_CLK_MASK;
					ERR = E_OK;
					break;
				case I2C_2:
					I2C_used  = I2C2;
					RCC->APB1ENR &= ~(I2C2_CLK_MASK);
					RCC->APB1ENR |= I2C2_CLK_MASK;
					ERR = E_OK;
					break;
				case I2C_3:
					I2C_used  = I2C3;
					RCC->APB1ENR &= ~(I2C3_CLK_MASK);
					RCC->APB1ENR |= I2C3_CLK_MASK;
					ERR = E_OK;
					break;
				default:
					ERR = E_INVALID_PARAMETER;
					break;
				}
				if(ERR == E_OK)
				{
					//Disable the PE
					I2C_used->I2C_CR1 &= ~(PERIPH_EN_MASK);

					/*Cfg CLK Stretching*/
					I2C_used->I2C_CR1 &= ~(ENNO_STRETCING_MASK);
					I2C_used->I2C_CR1 |= (I2C_Cfg[i].ClkStretchingEN << 7);


					/*Cfg GC*/
					I2C_used->I2C_CR1 &= ~(ENGC_MASK);
					I2C_used->I2C_CR1 |= (I2C_Cfg[i].GeneralCallEN << 6);

					/*Cfg PEC*/
					I2C_used->I2C_CR1 &= ~(ENPEC_MASK);
					I2C_used->I2C_CR1 |= (I2C_Cfg[i].PEC_EN << 5);

					/*Cfg FREQ*/
					I2C_used->I2C_CR2 &= ~(FREQ_MASK);
					I2C_used->I2C_CR2 |= (FREQ);

					/*Cfg BUF interrupt*/
					I2C_used->I2C_CR2 &= ~(ITBUFEN_MASK);
					I2C_used->I2C_CR2 |= (I2C_Cfg[i].BufferInterruptEN << 10);

					/*Cfg Event Interrupt*/
					I2C_used->I2C_CR2 &= ~(ITEVENTEN_MASK);
					I2C_used->I2C_CR2 |= (I2C_Cfg[i].EventInterruptEN << 9);

					/*Cfg Error Interrupt*/
					I2C_used->I2C_CR2 &= ~(ITERREN_MASK);
					I2C_used->I2C_CR2 |= (I2C_Cfg[i].ErrorInterruptEN << 8);

					/*Assign the 7 bits of Slave address*/
					I2C_used->I2C_OAR1 &= ~(SLAVEADD_7_1_MASK);
					I2C_used->I2C_OAR1 |=  (I2C_Cfg[i].SlaveAddress7BitsOAR1 << 1);

					if(I2C_Cfg[i].InterfaceAddMode ==  _10_BIT_ADDRESS)
					{
						/*Assign the Bit0 of the 10-Bit of Slave address*/
						I2C_used->I2C_OAR1 &= ~(ADD0_MASK);
						I2C_used->I2C_OAR1 |=  (I2C_Cfg[i].SlaveAddress10Bit_B0);
						/*Assign the Bit9 and Bit8 of the 10-Bit of Slave address*/
						I2C_used->I2C_OAR1 &= ~(ADD_9_8_MASK);
						I2C_used->I2C_OAR1 |=  (I2C_Cfg[i].SlaveAddress10Bit_B8_B9 << 8);

					}
					else
					{
						/*Assign the 7 bits of Slave address*/
						I2C_used->I2C_OAR2 &= ~(ENDUAL_MASK);
						I2C_used->I2C_OAR2 |=  (I2C_Cfg[i].DualModeEN);
						if(I2C_Cfg[i].DualModeEN == DUAL_ADDRESS_ENABLE)
						{
							/*Assign the 7 bits of Slave address*/
							I2C_used->I2C_OAR2 &= ~(SLAVEADD_7_1_MASK);
							I2C_used->I2C_OAR2 |=  (I2C_Cfg[i].SlaveAddress7BitsOAR2 << 1);
						}

					}

					/*Cfg The F/S Mode*/
					I2C_used->I2C_CCR &= ~(FSM_MASK);
					I2C_used->I2C_CCR |=  (I2C_Cfg[i].MasterModeSelection << 15);

					/*Cfg The DUTY*/
					I2C_used->I2C_CCR &= ~(DUTY_MASK);
					I2C_used->I2C_CCR |=  (I2C_Cfg[i].Duty << 14);

					/*Cfg The CCR*/
					I2C_used->I2C_CCR &= ~(CCR_MASK);
					//Put the equation
					u8calculatedPrescale = (FREQ*1000)/I2C_Cfg[i].CLK;
					if(I2C_Cfg[i].MasterModeSelection == DUTY_2TLOW_1THIGH_ENABLE)
					{
						u8CCR_Calc = u8calculatedPrescale / (THIGH_DUTY0+TLOW_DUTY0);
					}
					else
					{
						u8CCR_Calc = u8calculatedPrescale / (THIGH_DUTY1+TLOW_DUTY1);
					}
					I2C_used->I2C_CCR |= (u8CCR_Calc);

					/*Cfg The MAX Rise Time*/
					if(I2C_Cfg[i].MasterModeSelection == SM_ENABLE)
					{
						u8TRISE_ = (FREQ)+1;
					}
					else
					{
						u8TRISE_ = (FREQ*300)/1000.0;
					}
					I2C_used->I2C_TRISE &= ~(TRISE_MASK);
					I2C_used->I2C_TRISE |=  (u8TRISE_);

					/************Check first on ANF****************/

					/*Cfg The Analog Noise Filter*/
					I2C_used->I2C_FLTR &= ~(ANOFF_MASK);
					I2C_used->I2C_FLTR |=  (I2C_Cfg[i].AnalogNoiseFilterEN);

					/*Cfg The Digital Noise Filter*/
					I2C_used->I2C_FLTR &= ~(DNF_MASK);
					I2C_used->I2C_FLTR |=  (I2C_Cfg[i].DigitalNoiseFilterVal);

					/*Enable peripheral*/
					I2C_used->I2C_CR1 |= PERIPH_EN_MASK;

					/*Cfg ACK*/
					I2C_used->I2C_CR1 &= ~(ACK_MASK);
					I2C_used->I2C_CR1 |= (I2C_Cfg[i].ACK_EN << 10);
					gau8I2CInitFlags[I2C_Cfg->I2C_ChannelNo] = TRUE;
				}
				else
				{
					break;
				}
			}
		}
	}


	return ERR;
}

/*
 *  Function Name: _I2C_SendDataMaster
 * Input: - Data         -> Pointer to Data bytes to be written by Master
 *        - DataSize     -> Length of the bytes to be written
 *        - I2Cx         -> I2C Channel Number
 *        - SlaveAddress -> The Slave address the master will deal with
 * Output:
 * Return: ERROR Status
 * Description: This Function used for Master to send data
 * */
ERROR_STATUS _I2C_SendDataMaster(uint8_t* Data,uint16_t DataSize,uint8_t I2Cx,uint8_t SlaveAddress)
{
	ERROR_STATUS ERR;
	/*Check on NULL ptr*/
	if(Data == NULL)
	{
		ERR = E_NULL_PTR;
	}
	else
	{
		/*Check on parameters*/
		if((I2Cx != I2C_1 && I2Cx != I2C_2 && I2Cx != I2C_3))
		{
			ERR = E_INVALID_PARAMETER;
		}
		else
		{
			/*Check if the module is initialized*/
			if(gau8I2CInitFlags[I2Cx] == FALSE)
			{
				ERR = E_UNINITIALIZED_MODULE;
			}
			else
			{
				str_I2C_t * I2C_used;
				SlaveAddress  = SlaveAddress << 1;
				SlaveAddress |= WRITE_MODE;
				switch(I2Cx)
				{
				case I2C_1:
					I2C_used  = I2C1;
					break;
				case I2C_2:
					I2C_used  = I2C2;
					break;
				case I2C_3:
					I2C_used  = I2C3;
					break;
				default:
					break;
				}
				/*Enable the ACK*/
				I2C_used->I2C_CR1 &= ~(ACK_MASK);
				I2C_used->I2C_CR1 |=  (ACK_ENABLE);

				/*Check whether the interrupt is enable or not*/
				if((I2C_used->I2C_CR1 & ITEVENTEN_MASK))
				{
					/*Send The Start Condition*/
					I2C_used->I2C_CR1 &= ~(START_MASK);
					I2C_used->I2C_CR1 |=  (START_MASK);
					/*Set the shared resource for interrupt*/
					I2C_MasterSetter(Data,DataSize,I2Cx,SlaveAddress);

				}
				else
				{
					/*Send The Start Condition*/
					_I2C_SendSB(I2C_used);
					/*Send Slave Address*/
					_I2C_SendAddress(SlaveAddress,I2C_used);
					_I2C_SendData(Data,DataSize,I2C_used);
					/*Send Stop Condition*/
					_I2C_SendSTOP(I2C_used);
				}
				ERR =E_OK;
			}
		}
	}

	return ERR;
}

/*
 * Function Name: _I2C_SendDataSlave
 * Input: - Data         -> Pointer to Data bytes to be written by Slave
 *        - DataSize     -> Length of the bytes to be written
 *        - I2Cx         -> I2C Channel Number
 * Output:
 * Return: ERROR Status
 * Description: This Function used for Slave to send data
 * */
ERROR_STATUS _I2C_SendDataSlave(uint8_t* Data,uint16_t DataSize,uint8_t I2Cx)
{
	ERROR_STATUS ERR;
	/*Check on NULL ptr*/
	if(Data == NULL)
	{
		ERR = E_NULL_PTR;
	}
	else
	{
		/*Check on parameters*/
		if((I2Cx != I2C_1 && I2Cx != I2C_2 && I2Cx != I2C_3))
		{
			ERR = E_INVALID_PARAMETER;
		}
		else
		{
			/*Check if the module is initialized*/
			if(gau8I2CInitFlags[I2Cx] == FALSE)
			{
				ERR = E_UNINITIALIZED_MODULE;
			}
			else
			{
				str_I2C_t * I2C_used;
				switch(I2Cx)
				{
				case I2C_1:
					I2C_used  = I2C1;
					break;
				case I2C_2:
					I2C_used  = I2C2;
					break;
				case I2C_3:
					I2C_used  = I2C3;
					break;
				default:
					I2C_used  = I2C1;
					break;
				}
				/*Check whether the interrupt is enable or not*/
				if((I2C_used->I2C_CR1 & ITEVENTEN_MASK))
				{
					/*Set the shared resource for interrupt*/
					I2C_SlaveSetter(Data,DataSize,I2Cx);
					/*Enable the interrupt*/
					I2C_used->I2C_CR2 &= ~(ITBUFEN_MASK);
					I2C_used->I2C_CR2 |= (INT_BUFFER_ENABLE << 10);
				}
				else
				{
					//Send the data
					_I2C_SendData(Data,DataSize,I2C_used);
				}
				ERR =E_OK;
			}
		}
	}
	return ERR;
}

/*
 * Function Name: _I2C_RecieveDataMaster
 * Input: - Data         -> Pointer to Data bytes to be read by Master
 *        - DataSize     -> Length of the bytes to be read
 *        - I2Cx         -> I2C Channel Number
 *        - SlaveAddress -> The Slave address the master will deal with
 * Output:
 * Return: ERROR Status
 * Description: This Function used for Master to read data
 * */
ERROR_STATUS _I2C_RecieveDataMaster(uint8_t* Data,uint16_t DataSize,uint8_t I2Cx,uint8_t SlaveAddress)
{
	ERROR_STATUS ERR;
	/*Check on NULL ptr*/
	if(Data == NULL)
	{
		ERR = E_NULL_PTR;
	}
	else
	{
		/*Check on parameters*/
		if((I2Cx != I2C_1 && I2Cx != I2C_2 && I2Cx != I2C_3))
		{
			ERR = E_INVALID_PARAMETER;
		}
		else
		{
			/*Check if the module is initialized*/
			if(gau8I2CInitFlags[I2Cx] == FALSE)
			{
				ERR = E_UNINITIALIZED_MODULE;
			}
			else
			{
				str_I2C_t * I2C_used;
				SlaveAddress  = SlaveAddress << 1;
				SlaveAddress |= READ_MODE;
				switch(I2Cx)
				{
				case I2C_1:
					I2C_used  = I2C1;
					break;
				case I2C_2:
					I2C_used  = I2C2;
					break;
				case I2C_3:
					I2C_used  = I2C3;
					break;
				default:
					I2C_used  = I2C1;
					break;
				}

				/*Check whether the interrupt is enable or not*/
				if((I2C_used->I2C_CR1 & ITEVENTEN_MASK))
				{
					/*Send The Start Condition*/
					I2C_used->I2C_CR1 &= ~(START_MASK);
					I2C_used->I2C_CR1 |=  (START_MASK);
					/*Enable the ACK*/
					I2C_used->I2C_CR1 &= ~(ACK_MASK);
					I2C_used->I2C_CR1 |=  (ACK_ENABLE);
					/*Set the shared resource for interrupt*/
					I2C_MasterSetter(Data,DataSize,I2Cx,SlaveAddress);

				}
				else
				{

					/*Send The Start Condition*/
					_I2C_SendSB(I2C_used);
					/*Enable the ACK*/
					I2C_used->I2C_CR1 &= ~(ACK_MASK);
					I2C_used->I2C_CR1 |=  (ACK_ENABLE);
					/*Send Slave Address*/
					_I2C_SendAddress(SlaveAddress,I2C_used);
					_I2C_RecieveData(Data,DataSize,I2C_used);

					/*Disable the ACK*/
					I2C_used->I2C_CR1 &= ~(ACK_MASK);
					I2C_used->I2C_CR1 |=  (ACK_DISABLE);

					/*Send Stop Condition*/
					_I2C_SendSTOP(I2C_used);
				}
				ERR =E_OK;

			}
		}
	}
	return ERR;
}

/*
 * Function Name: _I2C_RecieveDataSlave
 * Input: - Data         -> Pointer to Data bytes to be read by Slave
 *        - DataSize     -> Length of the bytes to be read
 *        - I2Cx         -> I2C Channel Number
 * Output:
 * Return: ERROR Status
 * Description: This Function used for Slave to receive data
 * */
ERROR_STATUS _I2C_RecieveDataSlave(uint8_t* Data,uint16_t DataSize,uint8_t I2Cx,uint8_t SlaveAddress)
{
	ERROR_STATUS ERR;
	/*Check on NULL ptr*/
	if(Data == NULL)
	{
		ERR = E_NULL_PTR;
	}
	else
	{
		/*Check on parameters*/
		if((I2Cx != I2C_1) && (I2Cx != I2C_2) && (I2Cx != I2C_3))
		{
			ERR = E_INVALID_PARAMETER;
		}
		else
		{
			/*Check if the module is initialized*/
			if(gau8I2CInitFlags[I2Cx] == FALSE)
			{
				ERR = E_UNINITIALIZED_MODULE;
			}
			else
			{
				str_I2C_t * I2C_used;
				switch(I2Cx)
				{
				case I2C_1:
					I2C_used  = I2C1;
					break;
				case I2C_2:
					I2C_used  = I2C2;
					break;
				case I2C_3:
					I2C_used  = I2C3;
					break;
				default:
					I2C_used  = I2C1;
					break;
				}
				/*Check whether the interrupt is enable or not*/
				if((I2C_used->I2C_CR1 & ITEVENTEN_MASK))
				{
					/*Set the shared resource for interrupt*/
					I2C_SlaveSetter(Data,DataSize,I2Cx);
				}
				else
				{
					//Receive the data
					_I2C_RecieveData(Data,DataSize,I2C_used);
				}
				ERR =E_OK;
			}
		}
	}
	return ERR;
}

/*
 * Function Name: I2C_MasterSetter
 * Input: - Data         -> Pointer to Data bytes to be written or read by Master
 *        - DataSize     -> Length of the bytes to be written or read by Master
 *        - I2Cx         -> I2C Channel Number
 *        - SlaveAddress -> The Slave address the master will deal with
 * Output:
 * Return: ERROR Status
 * Description: This Function used for Saving the Input  Data from the Master to the shared Resources to be used in Interrupt
 * */
static void I2C_MasterSetter(uint8_t * Data,uint16_t DataSize, uint8_t I2Cx,uint16_t SlaveAddress)
{
	/*Take the Data in the Shared Resources*/
	if(gau8I2CBufferLocker[I2Cx] == UNLOCKED)
	{
		gau16I2CSlaveAddress[I2Cx] = SlaveAddress;
		gau8I2CBuffer[I2Cx] = Data;
		gau16I2CBufferCounter[I2Cx] =0;
		gau16I2CBufferSize[I2Cx] = DataSize;
		gau8I2CBufferLocker[I2Cx] = LOCKED;

	}
}

/*
 * Function Name: I2C_SlaveSetter
 * Input: - Data         -> Pointer to Data bytes to be written or read by Slave
 *        - DataSize     -> Length of the bytes to be written or read by Slave
 *        - I2Cx         -> I2C Channel Number
 * Output:
 * Return: ERROR Status
 * Description: This Function used for Saving the Input  Data from the Slave to the shared Resources to be used in Interrupt
 * */
static void I2C_SlaveSetter(uint8_t * Data,uint16_t DataSize, uint8_t I2Cx)
{
	/*Take the Data in the Shared Resources*/
	if(gau8I2CBufferLocker[I2Cx] == UNLOCKED)
	{
		gau8I2CBuffer[I2Cx] = Data;
		gau16I2CBufferCounter[I2Cx] =0;
		gau16I2CBufferSize[I2Cx] = DataSize;
		gau8I2CBufferLocker[I2Cx] = LOCKED;
	}
}

/*
 * Function Name: _I2C_SendSB
 * Input: - I2Cptr -> Pointer to the selected I2C Channel
 * Output:
 * Return: ERROR Status
 * Description: This Function used for sending Start bit by polling
 * */
static void _I2C_SendSB(str_I2C_t *I2Cptr)
{
	if(!(I2Cptr->I2C_SR2 & BUSY_MASK))
	{
		I2Cptr->I2C_CR1 &= ~(START_MASK);
		I2Cptr->I2C_CR1 |=  (START_MASK);
		while(!(I2Cptr->I2C_SR2 & BUSY_MASK));
	}
}

/*
 * Function Name: _I2C_SendSTOP
 * Input: - I2Cptr -> Pointer to the selected I2C Channel
 * Output:
 * Return: ERROR Status
 * Description: This Function used for sending Stop bit by polling
 * */
static void _I2C_SendSTOP(str_I2C_t *I2Cptr)
{
	if(!(I2Cptr->I2C_SR1 & STOPF_MASK))
	{
		I2Cptr->I2C_CR1 &= ~(STOPF_MASK);
		I2Cptr->I2C_CR1 |=  (STOPF_MASK);
	}
}


/*
 * Function Name: _I2C_SendAddress
 * Input: - I2Cptr  -> Pointer to the selected I2C Channel
 * 		  - Address -> Slave address to be sent
 * Output:
 * Return: ERROR Status
 * Description: This Function used for sending Slave address by polling
 * */
static void _I2C_SendAddress(uint8_t Address,str_I2C_t *I2Cptr)
{
	I2Cptr->I2C_DR = Address;
	while(!(I2Cptr->I2C_SR1 & ADDR_MASK));
	I2Cptr->I2C_SR1 &= ~(ADDR_MASK);

}

/*
 * Function Name: _I2C_SendData
 * Input: - Data         -> Pointer to Data bytes to be written by Master or Slave
 *        - DataSize     -> Length of the bytes to be written by Master or Slave
 *        - I2Cptr       -> Pointer to the selected I2C Channel
 * Output:
 * Return: ERROR Status
 * Description: This Function used for sending data using polling
 * */
static void _I2C_SendData(uint8_t * Data, uint16_t DataSize,str_I2C_t *I2Cptr)
{
	uint16_t u8iterator=0;
	for(u8iterator =0 ; u8iterator < DataSize; u8iterator++)
	{
		while(!(I2Cptr->I2C_SR1 & TXE_MASK));
		I2Cptr->I2C_DR = Data[u8iterator];

	}
}

/*
 * Function Name: _I2C_RecieveData
 * Input: - Data         -> Pointer to Data bytes to be read by Master or Slave
 *        - DataSize     -> Length of the bytes to be read by Master or Slave
 *        - I2Cptr       -> Pointer to the selected I2C Channel
 * Output:
 * Return: ERROR Status
 * Description: This Function used for receiving data using polling
 * */
static void _I2C_RecieveData(uint8_t * Data, uint16_t DataSize,str_I2C_t *I2Cptr)
{
	uint16_t u8iterator=0;
		for(u8iterator =0 ; u8iterator < DataSize; u8iterator++)
		{
			while(!(I2Cptr->I2C_SR1 & RXNE_MASK));
			Data[u8iterator] = I2Cptr->I2C_DR;
			if(u8iterator == DataSize-1)
			{
				if((I2Cptr->I2C_SR1 & MSL_MASK))
				{
					//Send the NOACK
					I2Cptr->I2C_CR1 &= ~(ACK_MASK);
					I2Cptr->I2C_CR1 |=  (ACK_DISABLE);

				}
			}
		}
}

/*I2C Handler*/
void I2C1_EV_IRQHandler(void)
{
	/*In case the start condition is sent*/
	if((I2C1->I2C_SR1 & SB_MASK))
	{
		/*Send the Slave Address*/
		I2C1->I2C_DR = gau16I2CSlaveAddress[I2C_1];
		I2C1->I2C_SR1 ;//clear flag
	}
	/*In case the Buffer is ready to write*/
	else if((I2C1->I2C_SR1 & TXE_MASK))
	{
		I2C1->I2C_DR = gau8I2CBuffer[I2C_1][gau16I2CBufferCounter[I2C_1]];
		gau16I2CBufferCounter[I2C_1]++;
		if(gau16I2CBufferCounter[I2C_1] == gau16I2CBufferSize[I2C_1])
		{
			gau16I2CBufferCounter[I2C_1]=0;
			gau8I2CBufferLocker[I2C_1]=UNLOCKED;
		}

	}
	/*In case the Buffer is ready to Read*/
	else if((I2C1->I2C_SR1 & RXNE_MASK))
	{
		gau8I2CBuffer[I2C_1][gau16I2CBufferCounter[I2C_1]] = I2C1->I2C_DR;
		gau16I2CBufferCounter[I2C_1]++;
		if(gau16I2CBufferCounter[I2C_1] == gau16I2CBufferSize[I2C_1])
		{
			gau16I2CBufferCounter[I2C_1]=0;
			gau8I2CBufferLocker[I2C_1]=UNLOCKED;
		}
	}
	/*In case the data transference is done*/
	if((I2C1->I2C_SR1 & BTF_MASK))
	{
		/*In case The data bytes transference is done and the buffer is unlocked */
		if(gau8I2CBufferLocker[I2C_1] == UNLOCKED)
		{
			/*Disable ACK*/
			I2C1->I2C_CR1 &= ~(ACK_MASK);
			I2C1->I2C_CR1 |=  (ACK_DISABLE);
			/*Send Stop condition*/
			if(!(I2C1->I2C_SR1 & STOPF_MASK))
			{
				I2C1->I2C_CR1 &= ~(STOPF_MASK);
				I2C1->I2C_CR1 |=  (STOPF_MASK);
			}
		}

	}
}

void I2C2_EV_IRQHandler(void)
{
	/*In case the start condition is sent*/
	if((I2C2->I2C_SR1 & SB_MASK))
	{
		/*Send the Slave Address*/
		I2C2->I2C_DR = gau16I2CSlaveAddress[I2C_2];
		I2C2->I2C_SR1 ;//clear flag
	}
	/*In case the Buffer is ready to write*/
	else if((I2C2->I2C_SR1 & TXE_MASK))
	{
		I2C2->I2C_DR = gau8I2CBuffer[I2C_2][gau16I2CBufferCounter[I2C_2]];
		gau16I2CBufferCounter[I2C_2]++;
		if(gau16I2CBufferCounter[I2C_2] == gau16I2CBufferSize[I2C_2])
		{
			gau16I2CBufferCounter[I2C_2]=0;
			gau8I2CBufferLocker[I2C_2]=UNLOCKED;
		}

	}
	/*In case the Buffer is ready to Read*/
	else if((I2C2->I2C_SR1 & RXNE_MASK))
	{
		gau8I2CBuffer[I2C_2][gau16I2CBufferCounter[I2C_2]] = I2C2->I2C_DR;
		gau16I2CBufferCounter[I2C_2]++;
		if(gau16I2CBufferCounter[I2C_2] == gau16I2CBufferSize[I2C_2])
		{
			gau16I2CBufferCounter[I2C_2]=0;
			gau8I2CBufferLocker[I2C_2]=UNLOCKED;
		}

	}
	/*In case the data transference is done*/
	if((I2C2->I2C_SR1 & BTF_MASK))
	{
		/*In case The data bytes transference is done and the buffer is unlocked */
		if(gau8I2CBufferLocker[I2C_2] == UNLOCKED)
		{
			/*Disable ACK*/
			I2C2->I2C_CR1 &= ~(ACK_MASK);
			I2C2->I2C_CR1 |=  (ACK_DISABLE);
			/*Send Stop condition*/
			if(!(I2C2->I2C_SR1 & STOPF_MASK))
			{
				I2C2->I2C_CR1 &= ~(STOPF_MASK);
				I2C2->I2C_CR1 |=  (STOPF_MASK);
			}
		}

	}

}

void I2C3_EV_IRQHandler(void)
{
	/*In case the start condition is sent*/
	if((I2C3->I2C_SR1 & SB_MASK))
	{
		/*Send the Slave Address*/
		I2C3->I2C_DR = gau16I2CSlaveAddress[I2C_3];
		I2C3->I2C_SR1 ;//clear flag
	}
	/*In case the Buffer is ready to write*/
	else if((I2C3->I2C_SR1 & TXE_MASK))
	{
		I2C3->I2C_DR = gau8I2CBuffer[I2C_3][gau16I2CBufferCounter[I2C_3]];
		gau16I2CBufferCounter[I2C_3]++;
		if(gau16I2CBufferCounter[I2C_3] == gau16I2CBufferSize[I2C_3])
		{
			gau16I2CBufferCounter[I2C_3]=0;
			gau8I2CBufferLocker[I2C_3]=UNLOCKED;
		}

	}
	/*In case the Buffer is ready to Read*/
	else if((I2C3->I2C_SR1 & RXNE_MASK))
	{
		gau8I2CBuffer[I2C_3][gau16I2CBufferCounter[I2C_3]] = I2C3->I2C_DR;
		gau16I2CBufferCounter[I2C_3]++;
		if(gau16I2CBufferCounter[I2C_3] == gau16I2CBufferSize[I2C_3])
		{
			gau16I2CBufferCounter[I2C_3] = 0;
			gau8I2CBufferLocker[I2C_3] = UNLOCKED;
		}

	}
	/*In case the data transference is done*/
	if((I2C3->I2C_SR1 & BTF_MASK))
	{
		/*In case The data bytes transference is done and the buffer is unlocked */
		if(gau8I2CBufferLocker[I2C_3] == UNLOCKED)
		{
			/*Disable ACK*/
			I2C3->I2C_CR1 &= ~(ACK_MASK);
			I2C3->I2C_CR1 |=  (ACK_DISABLE);
			/*Send Stop condition*/
			if(!(I2C3->I2C_SR1 & STOPF_MASK))
			{
				I2C3->I2C_CR1 &= ~(STOPF_MASK);
				I2C3->I2C_CR1 |=  (STOPF_MASK);
			}
		}

	}
}

