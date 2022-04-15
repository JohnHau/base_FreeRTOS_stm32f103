/**
 * @file lv_drv_conf.h
 * Configuration file for v7.11.0
 */

/*
 * COPY THIS FILE AS lv_drv_conf.h
 */

#if 1 /*Set it to "1" to enable the content*/

#ifndef LV_DRV_CONF_H
#define LV_DRV_CONF_H

#include "lv_conf.h"

/*********************
 * DELAY INTERFACE
 *********************/
#define LV_DRV_DELAY_INCLUDE  <stdint.h>            /*Dummy include by default*/
#define LV_DRV_DELAY_US(us)  /*delay_us(us)*/       /*Delay the given number of microseconds*/
#define LV_DRV_DELAY_MS(ms)  /*delay_ms(ms)*/       /*Delay the given number of milliseconds*/

/*********************
 * DISPLAY INTERFACE
 *********************/

/*------------
 *  Common
 *------------*/
#define LV_DRV_DISP_INCLUDE         <stdint.h>           /*Dummy include by default*/
#define LV_DRV_DISP_CMD_DATA(val)  /*pin_x_set(val)*/    /*Set the command/data pin to 'val'*/
#define LV_DRV_DISP_RST(val)       /*pin_x_set(val)*/    /*Set the reset pin to 'val'*/

/*---------
 *  SPI
 *---------*/
#define LV_DRV_DISP_SPI_CS(val)          /*spi_cs_set(val)*/     /*Set the SPI's Chip select to 'val'*/
#define LV_DRV_DISP_SPI_WR_BYTE(data)    /*spi_wr(data)*/        /*Write a byte the SPI bus*/
#define LV_DRV_DISP_SPI_WR_ARRAY(adr, n) /*spi_wr_mem(adr, n)*/  /*Write 'n' bytes to SPI bus from 'adr'*/

/*------------------
 *  Parallel port
 *-----------------*/
#define LV_DRV_DISP_PAR_CS(val)          /*par_cs_set(val)*/   /*Set the Parallel port's Chip select to 'val'*/
#define LV_DRV_DISP_PAR_SLOW             /*par_slow()*/        /*Set low speed on the parallel port*/
#define LV_DRV_DISP_PAR_FAST             /*par_fast()*/        /*Set high speed on the parallel port*/
#define LV_DRV_DISP_PAR_WR_WORD(data)    /*par_wr(data)*/      /*Write a word to the parallel port*/
#define LV_DRV_DISP_PAR_WR_ARRAY(adr, n) /*par_wr_mem(adr,n)*/ /*Write 'n' bytes to Parallel ports from 'adr'*/

/***************************
 * INPUT DEVICE INTERFACE
 ***************************/

/*----------
 *  Common
 *----------*/
#define LV_DRV_INDEV_INCLUDE     <stdint.h>             /*Dummy include by default*/
#define LV_DRV_INDEV_RST(val)    /*pin_x_set(val)*/     /*Set the reset pin to 'val'*/
#define LV_DRV_INDEV_IRQ_READ    0 /*pn_x_read()*/      /*Read the IRQ pin*/

/*---------
 *  SPI
 *---------*/
#define LV_DRV_INDEV_SPI_CS(val)            /*spi_cs_set(val)*/     /*Set the SPI's Chip select to 'val'*/
#define LV_DRV_INDEV_SPI_XCHG_BYTE(data)    0 /*spi_xchg(val)*/     /*Write 'val' to SPI and give the read value*/

/*---------
 *  I2C
 *---------*/
#define LV_DRV_INDEV_I2C_START              /*i2c_start()*/       /*Make an I2C start*/
#define LV_DRV_INDEV_I2C_STOP               /*i2c_stop()*/        /*Make an I2C stop*/
#define LV_DRV_INDEV_I2C_RESTART            /*i2c_restart()*/     /*Make an I2C restart*/
#define LV_DRV_INDEV_I2C_WR(data)           /*i2c_wr(data)*/      /*Write a byte to the I1C bus*/
#define LV_DRV_INDEV_I2C_READ(last_read)    0 /*i2c_rd()*/        /*Read a byte from the I2C bud*/


/*********************
 *  DISPLAY DRIVERS
 *********************/

/*-------------------------------------------------
 *  ILI9341 240X320 TFT LCD
 *------------------------------------------------*/
#ifndef USE_ILI9341
#  define USE_ILI9341       0
#endif

#if USE_ILI9341
#  define ILI9341_HOR_RES       240//LV_HOR_RES
#  define ILI9341_VER_RES       320//LV_VER_RES
#  define ILI9341_GAMMA         1
#  define ILI9341_TEARING       0
#endif  /*USE_ILI9341*/




/*********************
 *  INPUT DEVICES
 *********************/

/*--------------
 *    XPT2046
 *--------------*/
#ifndef USE_XPT2046
#  define USE_XPT2046         0
#endif

#if USE_XPT2046
#  define XPT2046_HOR_RES     320//480
#  define XPT2046_VER_RES     240//320
#  define XPT2046_X_MIN       200
#  define XPT2046_Y_MIN       200
#  define XPT2046_X_MAX       3800
#  define XPT2046_Y_MAX       3800
#  define XPT2046_AVG         4
#  define XPT2046_X_INV       0
#  define XPT2046_Y_INV       0
#  define XPT2046_XY_SWAP     0
#endif




#endif  /*LV_DRV_CONF_H*/

#endif /*End of "Content enable"*/
