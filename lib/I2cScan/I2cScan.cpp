

#define SDA_PIN 7
#define SCL_PIN 6 
#define I2C_SPEED 100000

#define MaxI2cBus 8
#define BaseDeviceBoardId 0x20
#define MaxDeviceBoardId  0x27

#define Config0 6
#define Config1 7

enum I2cBoardTypes = {BoardInput, BoardMixed, BoardOutput};

uint8_t I2cAdressVector[ MaxI2cBus];
uint8_t I2cTypeVector[ MaxI2cBus];
Adafruit_MCP23X08 I2cBoard[ MaxI2cBus];

// scaneia o barramento e configura os i/os
uint8_t I2cScan(void)
{
	uint8_t i, j;
	
	ESP_LOGI(TAG, "I2c Boards Scan..."); 
  
    uint8_t TotalDeviceNumber = 0; 
	
    Wire.begin(SDA_PIN, SCL_PIN, I2C_SPEED); 
	// varre os 8 endereços possíveis.
    for (i = 0; i < MaxI2cBus; i++)
    {
        
        Wire.beginTransmission(i + BaseDeviceBoardId);
		// testa se a placa está presente.
        if(Wire.endTransmission() == 0)
        {
            ESP_LOGI(TAG, "I2c Board Found: %d", i + BaseDeviceBoardId);                      
            I2cBoard[i].begin_I2C( i + BaseDeviceBoardId, &Wire);
            I2cBoard[i].pinMode(Config0, INPUT);
            I2cBoard[i].pinMode(Config1, INPUT);
			
            if((I2cBoard[i].digitalRead(Config0) == false) && (I2cBoard[i].digitalRead(Config1) == false))
            {
				ESP_LOGI(TAG, "I2c Board Type: Input");  
                I2cAdressVector[TotalDeviceNumber] = i + BaseDeviceBoardId;
				I2cTypeVector[TotalDeviceNumber++] = BoardInput;
				
				// aqui configura todos os pinos como entrada.
				for(j = 0; j < 6; j++)
				{					
					I2cBoard[i].pinMode(j, INPUT);
				}
            } 

			// testa se é do tipo mixedBoard ??? conferir.
            else if((I2cBoard[i].digitalRead(Config0) == false) && (I2cBoard[i].digitalRead(Config1) == true))
            {
				ESP_LOGI(TAG, "I2c Board Type: Mixed");
                I2cAdressVector[TotalDeviceNumber] = i + BaseDeviceBoardId;
				I2cTypeVector[TotalDeviceNumber++] = BoardMixed;
				
				// aqui configura metade dos pinos como entrada e a outra como saida.
				for(j = 0; j < 3; j++)
				{					
					I2cBoard[i].pinMode(j, INPUT);
					I2cBoard[i].pinMode(j + 3, OUTPUT);
				}
            }
			
			// testa se é do tipo outputBoard ??? conferir.
            else if((I2cBoard[i].digitalRead(Config0) == true) && (I2cBoard[i].digitalRead(Config1) == true))
            {
				ESP_LOGI(TAG, "I2c Board Type: Output");
                I2cAdressVector[TotalDeviceNumber] = i + BaseDeviceBoardId;
				I2cTypeVector[TotalDeviceNumber++] = BoardOutput;
				
				// aqui configura metade dos pinos como entrada e a outra como saida.
				for(j = 0; j < 6; j++)
				{					
					I2cBoard[i].pinMode(j, OUTPUT);
				}
            }
        }
    }
	// retorna dom o numero total de placas encontradas.
    return(TotalDeviceNumber);
}

uint8_t InputsDigitalRead(uint8_t pin)
{
	uint8_t totalInputs = 0;
	
	// tirar para outra posição ou manter
	for(uint8_t i; i < MaxI2cBus; i++)
	{
		if(I2cTypeVector[i] == BoardInput)
		{
			totalInputs += 8;
		}
	}
	
	if((totalInputs > 0) && (pin <= totalInputs))
	{

		if(pin < 6)
		{
			return(I2cBoard[pin / 6].digitalRead(pin + 0));
		}
		else 
		if(pin < 12)
		{
			return(I2cBoard[pin / 6].digitalRead(pin + 2));
		}
		else 
		if(pin < 16)
		{
			return(I2cBoard[pin / 6].digitalRead(pin + 4));
		}
		else 
		if(pin < 24)
		{
			return(I2cBoard[pin / 6].digitalRead(pin + 6));
		}
		else 
		if(pin < 30)
		{
			return(I2cBoard[pin / 6].digitalRead(pin + 8));
		}
		else 
		if(pin < 36)
		{
			return(I2cBoard[pin / 6].digitalRead(pin + 10));
		}
		else 
		if(pin < 42)
		{
			return(I2cBoard[pin / 6].digitalRead(pin + 12));
		}
		else 
		if(pin < 48)
		{
			return(I2cBoard[pin / 6].digitalRead(pin + 14));
		}
	
	}
	else
	{
		// erro: não existe a entrada
		return(255);
	}
}

uint8_t MixedDigitalRead(uint8_t pin)
{
	uint8_t totalMixed = 0;
	
	// tirar para outra posição ou manter
	for(uint8_t i; i < MaxI2cBus; i++)
	{
		if(I2cTypeVector[i] == BoardMixed)
		{
			totalMixed += 8;
		}
	}
	
	if((totalMixed > 0) && (pin <= totalMixed))
	{
		if(pin < 3)
		{
			return(I2cBoard[pin / 6].digitalRead(pin + 0));
		}
		else 
		if(pin < 6)
		{
			return(I2cBoard[pin / 6].digitalRead(pin + 5));
		}
		else 
		if(pin < 9)
		{
			return(I2cBoard[pin / 6].digitalRead(pin + 10));
		}
		else 
		if(pin < 12)
		{
			return(I2cBoard[pin / 6].digitalRead(pin + 15));
		}
		else 
		if(pin < 15)
		{
			return(I2cBoard[pin / 6].digitalRead(pin + 20));
		}
		else 
		if(pin < 18)
		{
			return(I2cBoard[pin / 6].digitalRead(pin + 25));
		}
		else 
		if(pin < 21)
		{
			return(I2cBoard[pin / 6].digitalRead(pin + 30));
		}
		else 
		if(pin < 24)
		{
			return(I2cBoard[pin / 6].digitalRead(pin + 35));
		}		
	}
	else
	{
		// erro: não existe a entrada
		return(255);
	}
}

uint8_t MixedDigitalWrite(uint8_t pin)
{
	uint8_t totalMixed = 0;
	
	// tirar para outra posição ou manter
	for(uint8_t i; i < MaxI2cBus; i++)
	{
		if(I2cTypeVector[i] == BoardMixed)
		{
			totalMixed += 8;
		}
	}
	
	if((totalMixed > 0) && (pin <= totalMixed))
	{
		if(pin < 3)
		{
			return(I2cBoard[pin / 6].digitalWrite(pin + 0 + 3));
		}
		else 
		if(pin < 6)
		{
			return(I2cBoard[pin / 6].digitalWrite(pin + 5 + 3));
		}
		else 
		if(pin < 9)
		{
			return(I2cBoard[pin / 6].digitalWrite(pin + 10 + 3));
		}
		else 
		if(pin < 12)
		{
			return(I2cBoard[pin / 6].digitalWrite(pin + 15 + 3));
		}
		else 
		if(pin < 15)
		{
			return(I2cBoard[pin / 6].digitalWrite(pin + 20 + 3));
		}
		else 
		if(pin < 18)
		{
			return(I2cBoard[pin / 6].digitalWrite(pin + 25 + 3));
		}
		else 
		if(pin < 21)
		{
			return(I2cBoard[pin / 6].digitalWrite(pin + 30 + 3));
		}
		else 
		if(pin < 24)
		{
			return(I2cBoard[pin / 6].digitalWrite(pin + 35 + 3));
		}		
	}
	else
	{
		// erro: não existe a entrada
		return(255);
	}
}

uint8_t OutputDigitalWrite(uint8_t pin)
{
	uint8_t totalOutput = 0;
	
	// tirar para outra posição ou manter
	for(uint8_t i; i < MaxI2cBus; i++)
	{
		if(I2cTypeVector[i] == BoardOutput)
		{
			totalOutput += 8;
		}
	}
	
	if((totalOutput > 0) && (pin <= totalOutput))
	{
		if(pin < 3)
		{
			return(I2cBoard[pin / 6].digitalWrite(pin + 0));
		}
		else 
		if(pin < 6)
		{
			return(I2cBoard[pin / 6].digitalWrite(pin + 2));
		}
		else 
		if(pin < 9)
		{
			return(I2cBoard[pin / 6].digitalWrite(pin + 4));
		}
		else 
		if(pin < 12)
		{
			return(I2cBoard[pin / 6].digitalWrite(pin + 8));
		}
		else 
		if(pin < 15)
		{
			return(I2cBoard[pin / 6].digitalWrite(pin + 10));
		}
		else 
		if(pin < 18)
		{
			return(I2cBoard[pin / 6].digitalWrite(pin + 12));
		}
		else 
		if(pin < 21)
		{
			return(I2cBoard[pin / 6].digitalWrite(pin + 14));
		}
		else 
		if(pin < 24)
		{
			return(I2cBoard[pin / 6].digitalWrite(pin + 16));
		}		
	}
	else
	{
		// erro: não existe a entrada
		return(255);
	}
}