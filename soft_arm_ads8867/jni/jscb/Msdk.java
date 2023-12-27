//Java Native Interface Java本地接口

package jscb;
		
public class Msdk
{
    static
    {
        System.loadLibrary("libmsdk");
    }

    public static native int  sdk_main_init();
    public static native int  sdk_main_start();
    
   @SuppressWarnings("static-access")
    public static void main(String[] args)
    { 
        Msdk msdk = new Msdk();
        msdk.sdk_main_init();
	msdk.sdk_main_start();
    }
}