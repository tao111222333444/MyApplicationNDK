package com.hugo.myapplicationndk;

import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.ImageView;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        //通过静态代码块加载so库
        System.loadLibrary("native-lib");
    }

    String textt = "明文abcdefg";
    String text22 ;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        text22 = stringFromJNIa(textt);
        // Example of a call to a native method
        TextView tv = (TextView) findViewById(R.id.sample_text);
        tv.setText(text22);

        tv = findViewById(R.id.sample_text1);
        tv.setText(stringFromJNIb(text22));

        ImageView iv1 = findViewById(R.id.iv_1);
        ImageView iv2 = findViewById(R.id.iv_2);
        iv1.setImageResource(R.mipmap.ic_launcher);
        //生成bitmap对象
        Bitmap bitmap = BitmapFactory.decodeResource(getResources(),R.mipmap.ic_launcher);
        //调用native方法，传入bitmap对象，进行高斯模糊处理
        gaussBlurBitmap(bitmap);
        //把bitmap对象设置给iv2
        iv2.setImageBitmap(bitmap);




    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();

    /**
     * 加密方法
     * @param aa
     * @return
     */
    public native String stringFromJNIa(String aa);

    /**
     * 解密方法
     * @param aa
     * @return
     */
    public native String stringFromJNIb(String aa);

    /**
     * 高斯模糊算法
     * @param bitmap
     */
    public native void gaussBlurBitmap(Bitmap bitmap);
}
