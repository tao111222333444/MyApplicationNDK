#include <jni.h>
#include <string>

//图片处理需要的库
// 需要在CMakeLists.txt 文件
// 这个方法里 target_link_libraries 添加-ljnigraphics
//target_link_libraries(native-lib -ljnigraphics ${log-lib})
#include <android/bitmap.h>
#include <math.h>
#include <stdlib.h>

extern "C" JNIEXPORT jstring

JNICALL
Java_com_hugo_myapplicationndk_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

//将java字符串转换为C语言字符串
char* Jstring2CStr(JNIEnv* env, jstring jstr){
    char* rtn = NULL;
    jclass clsstring = env -> FindClass("java/lang/String");
    jstring strencode = env -> NewStringUTF("UTF-8");
    jmethodID   mid   =   env->GetMethodID(clsstring,   "getBytes",   "(Ljava/lang/String;)[B"); //getBytes(Str);
    jbyteArray   barr=   (jbyteArray)env->CallObjectMethod(jstr,mid,strencode); // String .getByte("GB2312");
    jsize   alen   =   env->GetArrayLength(barr);
    jbyte*   ba   =   env->GetByteArrayElements(barr,JNI_FALSE);
    if(alen   >   0)
    {
        rtn   =   (char*)malloc(alen+1);         //"\0"
        memcpy(rtn,ba,alen);
        rtn[alen]=0;
    }
    env->ReleaseByteArrayElements(barr,ba,0);  //释放内存空间
    return rtn;
}



//单个字符异或运算
char MakecodeChar(char c, int key){
    return c=c^key;
}

//单个字符解密
char CotcodeChar(char c,int key){
    return c^key;
}
/**
 * 加密字符
 */
static int key[] = {1,2,3,4,5,6};

//加密
char* Makecode(char* pstr){
    //获取长度
    int len=strlen(pstr);
    for(int i = 0 ; i<len;i++){
        *(pstr + i) = MakecodeChar(*(pstr+i),key[i%5]);
    }
    return pstr;
}
//解密
char* Cutecode(char* pstr){
    //获取长度
    int len=strlen(pstr);
    for(int i = 0 ; i<len;i++){
        *(pstr + i) = CotcodeChar(*(pstr+i),key[i%5]);
    }
    return pstr;
}

//加密方法
extern "C" JNIEXPORT jstring

JNICALL
Java_com_hugo_myapplicationndk_MainActivity_stringFromJNIa(
        JNIEnv *env,
        jobject obj,jstring string) {
    char* cstr = Jstring2CStr(env,string);
    cstr = Makecode(cstr);
    return env->NewStringUTF(cstr);
}

//解密方法
extern "C" JNIEXPORT jstring
JNICALL
Java_com_hugo_myapplicationndk_MainActivity_stringFromJNIb(
        JNIEnv *env,
jobject obj,jstring string) {
char* cstr = Jstring2CStr(env,string);
cstr = Cutecode(cstr);
return env->NewStringUTF(cstr);
}


#define PI 3.14

void gaussBlur1(int* pix,int w , int h , int radius){
    float sigma = (float)(1.0 * radius / 2.57);
    float deon = (float)(1.0 / (sigma * sqrt(2.0 * M_PI)));
    float name = (float)(-1.0 / (2.0 * sigma * sigma));
    float* gaussMatrix = (float*) malloc(sizeof(float)*(radius + radius + 1));
    float gaussSum = 0.0;
    for (int i = 0,x = -radius; i <= radius ;++x, ++i) {
        float  g = (float) (deon * exp(1.0 * name * x * x));
        gaussMatrix[i] = g;
        gaussSum += g;
    }
    int len = radius + radius +1;
    for (int i = 0; i <  len; ++i) {
        gaussMatrix[i] /= gaussSum;
    }
    int* rowData =(int*)  malloc(w * sizeof(int));
    int* listData = (int*) malloc(h * sizeof(int));

    for (int y = 0; y < h; ++y) {
        memcpy(rowData,pix + y * w, sizeof(int) * w);
        for (int x = 0; x < w; ++x) {
            float r = 0 ,g = 0 , b = 0;
            gaussSum = 0;
            for (int i = -radius; i <= radius ; ++i) {
                int k = x+i;
                if(0<= k && k<= w){
                    //得到像素点的rgb值
                    int color = rowData[k];
                    int cr = (color & 0x00ff0000)>> 16;
                    int cg = (color & 0x0000ff00) >> 8;
                    int cb = (color & 0x000000ff) ;
                    r+= cr * gaussMatrix[i+ radius];
                    g += cg * gaussMatrix[i + radius];
                    b += cb * gaussMatrix[i + radius];
                    gaussSum+=gaussMatrix[i + radius];
                }
            }
            int cr = (int )(r/gaussSum);
            int cg = (int )(g / gaussSum);
            int cb = (int)(b/gaussSum);
            pix[y * w + x] = cr << 16 | cg << 8 | cb | 0xff000000;
        }
    }
    for (int x = 0; x < w ; ++x) {
        for(int y = 0; y<h;++y){
            listData[y] = pix[y * w +x];
        }
        for (int y = 0; y < h; ++y) {
            float r = 0,g =0,b =0;
            gaussSum = 0;
            for (int j = -radius; j <= radius; ++j) {
                int k= y+j;
                if(0 <= k && k<= h){
                    int color = listData[k];
                    int cr = (color & 0x00ff0000)>>16;
                    int cg = (color & 0x0000ff00)>>8;
                    int cb = (color & 0x000000ff);
                    r += cr * gaussMatrix[j + radius];
                    g+= cg * gaussMatrix[j + radius];
                    b += cb * gaussMatrix[j + radius];
                    gaussSum += gaussMatrix[j + radius];
                }
            }
            int cr = (int) (r/gaussSum);
            int cg = (int)(g/gaussSum);
            int cb = (int)(b/gaussSum);
            pix[y * w +x]=cr <<16 | cg << 8 |cb |0xff000000;
        }
    }
    free(gaussMatrix);
    free(rowData);
    free(listData);
}

extern "C" JNIEXPORT void
JNICALL
Java_com_hugo_myapplicationndk_MainActivity_gaussBlurBitmap(
        JNIEnv *env,
        jobject /*obj*/,jobject bmp) {
    //AndroidBitmap_getInfo() 从位图句柄获得信息（宽度、高度、像素格式）
    //AndroidBitmap_lockPixels() 对像素缓存上锁，即获得该缓存的指针。
    //AndroidBitmap_unlockPixels() 解锁

    //初始化bitmapInfo结构体
    AndroidBitmapInfo info ;
    //初始化bitmap图像指针
    int *data = NULL;

    AndroidBitmap_getInfo(env,bmp,&info);
    //锁定bitmap，并且获得指针
    AndroidBitmap_lockPixels(env, bmp, reinterpret_cast<void **>(&data));
    /******************高斯模糊算法作对int数组进行处理******************/
    //调用gaussBlur函数，把图像数据指针、图片长宽和模糊半截传入
    gaussBlur1(data,info.width,info.height,80);

    //解锁
    AndroidBitmap_unlockPixels(env,bmp);

}




