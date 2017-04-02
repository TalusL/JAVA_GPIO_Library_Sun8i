package GPIO.Sun8i;
import GPIO.Sun8i.OrangePi.OPIZeroPin;

public class Demo {

    public static void main(String[] args) throws InterruptedException {
	    GPIOCtrl.gpio_setcfg(OPIZeroPin.STATUSIOLED,GPIO.ModelOutput);
	    while(true){
            GPIOCtrl.gpio_output(OPIZeroPin.STATUSIOLED,GPIO.StatusHIGH);
            Thread.sleep(500);
            GPIOCtrl.gpio_output(OPIZeroPin.STATUSIOLED,GPIO.StatusLOW);
            Thread.sleep(500);
        }

    }
}
