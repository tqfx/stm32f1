#include "spi_s.h"

uint8_t SPI_WriteRead8(uint8_t txData)
{
    uint8_t rxData = 0U;

    for (uint8_t i = 0U; i < 8U; i++)
    {
        SPI_SCK_0;
        SPI_DELAY;
        //TransmitData

        if (txData & 0x80U)
            SPI_MOSI_1;
        else
            SPI_MOSI_0;

        txData <<= 1U;
        SPI_DELAY;

        SPI_SCK_1;
        SPI_DELAY;
        //ReceiveData
        rxData <<= 1U;
        if (SPI_READ_MISO)
            rxData |= 0x01U;

        SPI_DELAY;
    }
    SPI_SCK_1;

    return rxData;
}