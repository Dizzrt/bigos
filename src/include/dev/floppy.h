#include "../port.h"

enum FloppyType
{
    dHD
};

// floppy_parm:扇区数、每磁道扇区数、磁头数、磁道数
//磁道特殊处理(stretch)、扇区间隙长度、数据传输速率、速度参数(spec1)
struct FloppyParm
{
    __UINT8_TYPE__ sectorCount, sectorPerTrack, headerCount, trackCount,
        stretch, gap, rate, spec1;
};
static FloppyParm floppy_parm[1] = {{2880, 18, 2, 80, 0, 0x1b, 0x00, 0xcf}};

extern "C"
{
    void floppy_init();
    void floppy_command(__INT8_TYPE__);
    void floppy_seek(__INT8_TYPE__, __INT8_TYPE__, __INT8_TYPE__);
    void floppy_recalibrate(__INT8_TYPE__);
    void floppy_DMA_init(__INT32_TYPE__, __INT16_TYPE__);

    void floppy_read(__INT8_TYPE__, __INT8_TYPE__, __INT8_TYPE__, __INT8_TYPE__, __INT32_TYPE__, __INT16_TYPE__, FloppyType);
}

/**
 * 初始化DMA通道
 * addr:数据存放地址 count:数据字节长度
 */
void floppy_DMA_init(__INT32_TYPE__ addr, __INT16_TYPE__ count)
{
    outb(0x0a, 0x06); // mask DMA channel 2
    outb(0x0c, 0xff); // reset the master flip-flop

    __INT8_TYPE__ temp_8bit = addr & 0xff;
    outb(0x04, temp_8bit); // address low byte

    addr >>= 8;
    temp_8bit = addr & 0xff;
    outb(0x04, temp_8bit); // address mid byte

    addr >>= 8;
    temp_8bit = addr & 0xff;
    outb(0x81, temp_8bit); // address high byte

    outb(0x0c, 0xff);

    temp_8bit = count & 0xff;
    outb(0x05, temp_8bit); // count low byte

    count >>= 8;
    temp_8bit = count & 0xff;
    outb(0x05, temp_8bit); // count high byte

    outb(0x0a, 0x02); // unmask DMA channel 2

    return;
}

//向0x3F5端口写入
void floppy_command(__INT8_TYPE__ value)
{
    __INT8_TYPE__ FDC_Status;
    do
    {
        FDC_Status = inb(0x3f4);
        FDC_Status &= 0x80;
    } while (FDC_Status != 0x80);

    outb(0x3f5, value);
    return;
}

//寻道
void floppy_seek(__INT8_TYPE__ header, __INT8_TYPE__ driver, __INT8_TYPE__ track)
{
    floppy_command(0x0f);
    floppy_command(header & driver);
    floppy_command(track);
    return;
}

//校准驱动器，磁头回到0磁道
void floppy_recalibrate(__INT8_TYPE__ driver)
{
    floppy_command(0x07);
    floppy_command(driver);
    return;
}

void floppy_init()
{
    outb(0x3f2, 0x00);
    outb(0x3f2, 0x1c);
    outb(0x3f7, 0);

    floppy_command(0x03);
    floppy_command(0xcf);
    floppy_command(0x06);
    return;
}

void floppy_read(__INT8_TYPE__ driver, __INT8_TYPE__ track,
                 __INT8_TYPE__ header, __INT8_TYPE__ sector, __INT32_TYPE__ addr,
                 __INT16_TYPE__ count, int, FloppyType floppyType = FloppyType::dHD)
{

    floppy_recalibrate(driver);
    floppy_seek(header, driver, track);
    floppy_DMA_init(addr, count);

    //----read----
    floppy_command(0xe6);
    floppy_command(driver);
    floppy_command(track);
    floppy_command(header);
    floppy_command(sector);

    floppy_command(0x02); // MARKER 每扇区字节数  N=2==>512bytes
    floppy_command(floppy_parm[floppyType].sectorPerTrack);
    floppy_command(floppy_parm[floppyType].gap);
    floppy_command(0xff); // N=0,指定扇区字节数为512
    //------------
}
