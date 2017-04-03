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
    public static native int  gpio_init();

    public static native int gpio_setcfg(int pin,int p1);

    public static native int gpio_getcfg(int pin);

    public static native int gpio_output(int pin,int p1);

    public static native int gpio_pullup(int pin,int p1);

    public static native int gpio_input(int pin);

}
