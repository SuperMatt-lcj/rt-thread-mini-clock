#include <rtthread.h>
#include <board.h>
#include <sht3x.h>

#define THREAD_PRIORITY         25
#define THREAD_STACK_SIZE       512
#define THREAD_TIMESLICE        5

static rt_thread_t tid1 = RT_NULL;

/* 入口函数 */
static void sht31_collect_thread_entry(void *parameter)
{
    sht3x_device_t  sht3x_device;

    sht3x_device = sht3x_init("i2c1", 0x44);

    //此处需要短暂的延时，否则会出现连续读取失败的问题
    rt_thread_mdelay(150);

    while (1)
    {
        if(RT_EOK == sht3x_read_singleshot(sht3x_device))
        {
            rt_kprintf("sht31 humidity   : %d.%d  ", (int)sht3x_device->humidity, (int)(sht3x_device->humidity * 10) % 10);
            rt_kprintf("temperature: %d.%d\n", (int)sht3x_device->temperature, (int)(sht3x_device->temperature * 10) % 10);
        }
        else
        {
            rt_kprintf("read sht3x fail.\r\n");
            break;
        }
        rt_thread_mdelay(2000);
    }
}

/* 创建线程 */
int sht31_collect(void)
{
    /* 创建线程*/
    tid1 = rt_thread_create("sht30_collect_thread",
            sht31_collect_thread_entry, RT_NULL,
                            THREAD_STACK_SIZE,
                            THREAD_PRIORITY, THREAD_TIMESLICE);

    /* 如果获得线程控制块，启动这个线程 */
    if (tid1 != RT_NULL)
        rt_thread_startup(tid1);

    return 0;
}
