package GPIO.Sun8i;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;

/**
 * Created by Wind on 2017/4/1.
 */
public class GPIOCtrl {
    static {
        try{
            //export and load native lib
            String saveDir = System.getProperty("user.dir") + "/.gpiolib";
            String libPath = saveDir+"/GPIO_Sun8i_GPIOCtrl.so";
            InputStream fin =  GPIOCtrl.class.getResourceAsStream("/GPIO_Sun8i_GPIOCtrl.so");
            byte[] b =new byte[fin.available()];
            fin.read(b);
            fin.close();
            File lib = new File(libPath);
            if(!lib.exists()){
                File libdir = new File(saveDir);
                if(!libdir.exists()) libdir.mkdirs();
                FileOutputStream fos = new FileOutputStream(libPath);
                fos.write(b);
                fos.flush();
                fos.flush();
            }
            System.load(libPath);

            //init gpio
            gpio_init();
        }catch (IOException e){
            e.printStackTrace();
        }

    }


    //native interfaces

    /**
     * init gpio
     * @return scuess 0 or not -1
     */
    public static native int  gpio_init();

    /**
     * config gpio model
     * @param gpioPin IO
     * @param gpioModel Input 0 or Output 1
     * @return scuess 0 or not -1
     */
    public static native int gpio_setcfg(int gpioPin,int gpioModel);

    /**
     * get gpio config
     * @param gpioPin gpioPin
     * @return Input 0 or Output 1
     */
    public static native int gpio_getcfg(int gpioPin);

    /**
     *
     * @param gpioPin gpioPin
     * @param gpioStatus gpioStatus Low 0 or High 1
     * @return scuess 0 or not -1
     */
    public static native int gpio_output(int gpioPin,int gpioStatus);

    /**
     * raise or down GpioPin
     * @param gpioPin gpioPin
     * @param gpioPut PutUP PutDOWN
     * @return scuess 0 or not -1
     */
    public static native int gpio_pullup(int gpioPin,int gpioPut);

    /**
     * read gpio status
     * @param gpioPin gpioPin
     * @return gpio status down 0  or up 1
     */
    public static native int gpio_input(int gpioPin);

}
