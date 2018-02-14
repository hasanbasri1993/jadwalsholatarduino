#include <DMD.h>
#include <TimerOne.h>
#include <Wire.h>
#include <Sodaq_DS3231.h>
#include "PrayerTimes.h"
#include <SystemFont5x7.h>
#include <angka6x13.h>
#include <angka_2.h>
#include <huruf_kecilku.h>
#include <SPI.h>
char weekDay[][9] = {
  "MONDAY",
  "TUESDAY",
  "WEDNESDAY",
  "THURSDAY",
  "FRIDAY",
  "SATURDAY",
  "SUNDAY",
  "MONDAY",
}; // array hari, dihitung mulai dari senin, hari senin angka nya =0,
int langkah;
int lama_tampilnya;

DMD dmd(5, 1); //untuk mengatur jumlah panel yang kamu pakai

double times[sizeof(TimeName) / sizeof(char *)];
double lintang = -6.727302; //Latitude
double bujur = 106.812802;  //Longitude
int Z = 7;                  //Zona Waktu GMT WIB biasanya 7

int showsubuh_x = 37, showsubuh_y = 0, showsubuh_x1 = 37, showsubuh_y1 = 9;         // variabel buat ngatur waktu subuh
int showdzuhur_x = 80, showdzuhur_y = 0, showdzuhur_x1 = 80, showdzuhur_y1 = 9;     // variabel buat ngatur waktu dzuhur
int showashar_x = 128, showashar_y = 0, showashar_x1 = 128, showashar_y1 = 9;       // variabel buat ngatur waktu asar
int showmaghrib_x = 32, showmaghrib_y = 0, showmaghrib_x1 = 36, showmaghrib_y1 = 9; // variabel buat ngatur waktu maghrib
int showisya_x = 80, showisya_y = 0, showisya_x1 = 83, showisya_y1 = 9;             // variabel buat ngatur isya
int showimsak_x = 127, showimsak_y = 0, showimsak_x1 = 127, showimsak_y1 = 9;       // variabel buat ngatur waktu imsak
int lamatimetoazan = 24000;                                                         //milisecond
int lamaiqomah = 10;                                                                 //menit

char* imam_subuh0 = "UST.DRS.H.AHMAD YANI", *imam_subuh1 = "UST.KAMALUDDIN", * imam_subuh2 = "UST.H.YALET.N", * imam_subuh3 = "UST.TAUFIQ ISMAIL", * imam_subuh4 = "UST.UJANG MUSA ";
int imam_subuh0pj = 19, imam_subuh1pj = 15, imam_subuh2pj = 13, imam_subuh3pj = 17, imam_subuh4pj = 14;
char *imam_magrib0 = "UST.DRS.H.AHMAD YANI", * imam_magrib1 = "UST.KAMALUDDIN", * imam_magrib2 = "UST.H.YALET.N", * imam_magrib3 = "UST.TAUFIQ ISMAIL", * imam_magrib4 = "UST.UJANG MUSA";
int imam_magrib0pj = 19, imam_magrib1pj = 15, imam_magrib2pj = 13, imam_magrib3pj = 17, imam_magrib4pj = 14;
char* imam_isya0 = "UST.DRS.H.AHMAD YANI", * imam_isya1 = "UST.KAMALUDDIN", * imam_isya2 = "UST.H.YALET.N", * imam_isya3 = "UST.TAUFIQ ISMAIL", * imam_isya4 = "UST.UJANG MUSA";
int imam_isya0pj = 19, imam_isya1pj = 15, imam_isya2pj = 13, imam_isya3pj = 17, imam_isya4pj = 14;
char *khotib0 = "UST.KAMALUDDIN", khotib1 = "UST.H.YALET.N", khotib2 = "UST.TAUFIQ ISMAIL", khotib3 = "UST.UJANG MUSA";
int imam_khotib0pj = 19, imam_khotib1pj = 15, imam_khotib2pj = 13, imam_khotib3pj = 17, imam_khotib4pj = 14;

char* iqomahtext = "IQOMAH";
int iqomah_x = 124, iqomah_y = 0, imam_x = 1, imam_y = 0, textimam_x = 1, textimam_y = 8;

String textjalannamamasjid = "AL-HADI DULIDO MOSQUE";
String textinfojalan = "DON'T BE NOISE - STRAIGHTEN AND CLOSE THE LINE ! { ROBOTIC STUDY CLUB }";

DateTime dt(2018, 2, 15, 4, 36, 50 , 3);
void ScanDMD()
{
  dmd.scanDisplayBySPI();
}

void setup()
{

  pinMode(2, OUTPUT); //----pin 2 untuk alarm adzan maghrib ---hubungkan dengan buzzer

  langkah = 0;
  rtc.begin();
  rtc.setDateTime(dt); //Adjust date-time as defined 'dt' above (ini untuk setting jam)
  Wire.begin();
  Timer1.initialize(500);
  Timer1.attachInterrupt(ScanDMD);
  dmd.clearScreen(true);
}
////////////////////////////////// Fungsi Looping/Pengulangan ///////////////////////////////////////
void loop()
{
  dmd.setKecerahan(60);
  mulai();
}

//////////////////////==========================Program Jam==============

void mulai()
{
  dmd.clearScreen(true);
  long mulai = millis();
  while (millis() - mulai < 10000)
  {

    DateTime now = rtc.now();
    String Teks;

    ///================pengaturan koordinat-gunakan gps android untuk menentukan posisi anda=================

    int tgl = now.date();  //Tanggal
    int bln = now.month(); //Bulan
    int thn = now.year();  //Tahun
    int hours, minutes;    //jam sholat dan  menit sholat

    long mulai = millis();

    set_calc_method(ISNA);  //Methode perhitungan mengikuti ISNA
    set_asr_method(Shafii); //Pendapat Imam Syafi'i
    set_high_lats_adjust_method(AngleBased);
    set_fajr_angle(20); //Sudut Waktu Subuh
    set_isha_angle(18); ////Sudut Waktu Isya
    get_prayer_times(thn, bln, tgl, lintang, bujur, Z, times);

    ///jam--besar///
    dmd.clearScreen(true);
    dmd.selectFont(angka6x13);
    int menit_e = now.minute();
    int jame = now.hour();
    int detik_e = now.second();
    String xjam = Konversi(jame); //tampilan jam
    int pjg = xjam.length() + 1;
    char sjam[pjg];
    xjam.toCharArray(sjam, pjg);
    dmd.drawString(1, 0, sjam, pjg, 0);

    /////menit///
    String xxjam = Konversi(menit_e); //tampilan menit
    int xpjg = xxjam.length() + 1;
    char xsjam[xpjg];
    xxjam.toCharArray(xsjam, xpjg);
    dmd.drawString(18, 0, xsjam, xpjg, 0);

    ///titik dua :

    dmd.selectFont(SystemFont5x7);
    dmd.drawChar(14, 4, ':', GRAPHICS_OR); // clock colon overlay on
    delay(100);

    dmd.drawChar(14, 4, ':', GRAPHICS_OR); // clock colon overlay off
    delay(100);

    ////subuh
    get_float_time_parts(times[0], hours, minutes);
    Teks = "SUBUH   "; //baris atas
    char pj = Teks.length() + 1;
    char tampil[pj];
    Teks.toCharArray(tampil, pj);
    dmd.selectFont(SystemFont5x7);
    dmd.drawString(showsubuh_x, showsubuh_y, tampil, pj, 0); //koordinat tampilan
    Teks = Konversi(hours) + ":" + Konversi(minutes);        //baris kedua
    pj = Teks.length() + 1;
    tampil[pj];
    Teks.toCharArray(tampil, pj);
    dmd.selectFont(SystemFont5x7);
    dmd.drawString(showsubuh_x1, showsubuh_y1, tampil, pj, 0); //koordinat tampilan
    dmd.drawChar(14, 4, ':', GRAPHICS_NOR);                    // clock colon overlay on
    delay(1000);

    dmd.drawChar(14, 4, ':', GRAPHICS_OR); // clock colon overlay off
    delay(1000);
    dmd.drawChar(14, 4, ':', GRAPHICS_NOR); // clock colon overlay on
    delay(1000);

    dmd.drawChar(14, 4, ':', GRAPHICS_OR); // clock colon overlay off
    delay(1000);
    /////////
    ////dzuhur

    get_float_time_parts(times[2], hours, minutes);
    Teks = "DZUHUR"; //baris atas
    pj = Teks.length() + 1;
    tampil[pj];
    Teks.toCharArray(tampil, pj);
    dmd.selectFont(SystemFont5x7);
    dmd.drawString(showdzuhur_x, showdzuhur_y, tampil, pj, 0); //koordinat tampilan
    Teks = Konversi(hours) + ":" + Konversi(minutes);          //baris kedua
    pj = Teks.length() + 1;
    tampil[pj];
    Teks.toCharArray(tampil, pj);
    dmd.selectFont(SystemFont5x7);
    dmd.drawString(showdzuhur_x1, showdzuhur_y1, tampil, pj, 0); //koordinat tampilan
    dmd.drawChar(14, 4, ':', GRAPHICS_NOR);                      // clock colon overlay on
    delay(1000);
    dmd.drawChar(14, 4, ':', GRAPHICS_OR); // clock colon overlay off
    delay(1000);

    ////Asar

    get_float_time_parts(times[3], hours, minutes);
    Teks = "ASHAR"; //baris atas
    pj = Teks.length() + 1;
    tampil[pj];
    Teks.toCharArray(tampil, pj);
    dmd.selectFont(SystemFont5x7);
    dmd.drawString(showashar_x, showashar_y, tampil, pj, 0); //koordinat tampilan
    Teks = Konversi(hours) + ":" + Konversi(minutes);        //baris kedua
    pj = Teks.length() + 1;
    tampil[pj];
    Teks.toCharArray(tampil, pj);
    dmd.selectFont(SystemFont5x7);
    dmd.drawString(showashar_x1, showashar_y1, tampil, pj, 0); //koordinat tampilan
    dmd.drawChar(14, 4, ':', GRAPHICS_NOR);                    // clock colon overlay on
    delay(1000);

    dmd.drawChar(14, 4, ':', GRAPHICS_OR); // clock colon overlay off
    delay(1000);
    /////////
    ////Maghrib

    get_float_time_parts(times[4], hours, minutes);
    Teks = "MAGHRIB"; //baris atas
    pj = Teks.length() + 1;
    tampil[pj];
    Teks.toCharArray(tampil, pj);
    dmd.selectFont(SystemFont5x7);
    dmd.drawString(showmaghrib_x, showmaghrib_y, tampil, pj, 0); //koordinat tampilan
    Teks = Konversi(hours) + ":" + Konversi(minutes);            //baris kedua
    pj = Teks.length() + 1;
    tampil[pj];
    Teks.toCharArray(tampil, pj);
    dmd.selectFont(SystemFont5x7);
    dmd.drawString(showmaghrib_x1, showmaghrib_y1, tampil, pj, 0); //koordinat tampilan
    dmd.drawChar(14, 4, ':', GRAPHICS_NOR);                        // clock colon overlay on
    delay(1000);

    dmd.drawChar(14, 4, ':', GRAPHICS_OR); // clock colon overlay off
    delay(1000);
    /////////
    ///Isya'

    get_float_time_parts(times[6], hours, minutes);
    Teks = " ISYA'"; //baris atas
    pj = Teks.length() + 1;
    tampil[pj];
    Teks.toCharArray(tampil, pj);
    dmd.selectFont(SystemFont5x7);
    dmd.drawString(showisya_x, showisya_y, tampil, pj, 0); //koordinat tampilan
    Teks = Konversi(hours) + ":" + Konversi(minutes);      //baris kedua
    pj = Teks.length() + 1;
    tampil[pj];
    Teks.toCharArray(tampil, pj);
    dmd.selectFont(SystemFont5x7);
    dmd.drawString(showisya_x1, showisya_y1, tampil, pj, 0); //koordinat tampilan
    dmd.drawChar(14, 4, ':', GRAPHICS_NOR);                  // clock colon overlay on
    delay(1000);

    dmd.drawChar(14, 4, ':', GRAPHICS_OR); // clock colon overlay off
    delay(1000);
    /////////
    //imsak

    get_float_time_parts(times[0], hours, minutes);
    if (minutes < 11)
    {
      minutes = 50 + minutes;
      hours--;
    }
    else
    {
      minutes = minutes - 10;
    }
    Teks = "IMSAK"; //baris atas
    pj = Teks.length() + 1;
    tampil[pj];
    Teks.toCharArray(tampil, pj);
    dmd.selectFont(SystemFont5x7);
    dmd.drawString(showimsak_x, showimsak_y, tampil, pj, 0); //koordinat tampilan
    Teks = Konversi(hours) + ":" + Konversi(minutes);        //baris kedua
    pj = Teks.length() + 1;
    tampil[pj];
    Teks.toCharArray(tampil, pj);

    dmd.selectFont(SystemFont5x7);
    dmd.drawString(showimsak_x1, showimsak_y1, tampil, pj, 0); //koordinat tampilan
    dmd.drawChar(14, 4, ':', GRAPHICS_NOR);                    // clock colon overlay on
    delay(1000);

    dmd.drawChar(14, 4, ':', GRAPHICS_OR); // clock colon overlay off
    delay(1000);
  }

  ////runing teks jam diatas----//

  dmd.clearScreen(true);
  while (millis() - mulai < 20000)
  {
    DateTime now = rtc.now();
    String Teks;
    int tgl = now.date();  //Tanggal
    int bln = now.month(); //Bulan
    int thn = now.year();  //Tahun
    // int hours, minutes;        //jam dan menit jawdwal sholat
    int jame = now.hour();      //jam
    int menit_e = now.minute(); //menit
    int detik_e = now.second(); //detik
    int harin_e = harin_e;      //hari
    delay(100);
    detachInterrupt(0); // Matikan interrupt "0"

    ////   // Tampilkan hari

    dmd.selectFont(SystemFont5x7);

    int dowo = textjalannamamasjid.length() + 1;
    char detikanku[dowo];
    textjalannamamasjid.toCharArray(detikanku, dowo);
    dmd.drawString(15, 0, detikanku, dowo, 0);

    int kecepatan;
    kecepatan = 50; //kecepatan runing teks
    int pj = textinfojalan.length() + 1;
    char tampil[pj];
    textinfojalan.toCharArray(tampil, pj);
    dmd.selectFont(SystemFont5x7);
    dmd.drawMarquee(tampil, pj, 160, 9);

    long timer = millis();
    boolean ret = false;
    while (!ret)
    {
      if ((timer + kecepatan) < millis())
      {
        ret = dmd.stepSplitMarquee(8, 15, 0); ///dianimasikan baris 8 sampai 15, sampai kolom 0
        timer = millis();
      }
    }
  }

  ///=============================PROGRAM ADZAN DAN IQOMAH===================
  //jam adzan Subuh----------------------------------------------------------
  int hours;
  int minutes;
  dmd.clearScreen(true);
  while (millis() - mulai < 80000)
  {
    ///////////////////////

    DateTime now = rtc.now();
    int harin_e = now.dayOfWeek();
    String hariku = Konversi(now.date()) + "/" + Konversi(now.month()) + "/" + Konversi(now.year()) + " "; //tampilan tanggal
    int dowo = hariku.length() + 1;
    char detikanku[dowo];
    hariku.toCharArray(detikanku, dowo);
    dmd.selectFont(SystemFont5x7);
    dmd.drawString(80, 5, detikanku, dowo, 0);

    String xjam = Konversi(now.hour()) + ";" + Konversi(now.minute()) + ";" + Konversi(now.second()); //tampilan jam
    int pjg = xjam.length() + 1;
    char sjam[pjg];
    xjam.toCharArray(sjam, pjg);
    dmd.selectFont(angka_2);
    dmd.drawString(5, 5, sjam, pjg, 0);

    get_float_time_parts(times[0], hours, minutes);
    if ((now.hour() == hours && now.minute() == minutes) || (now.hour() == hours && now.minute() == minutes + 1) || (now.hour() == hours && now.minute() == minutes + 2) || (now.hour() == hours && now.minute() == minutes + 3) || (now.hour() == hours && now.minute() == minutes + 4) || (now.hour() == hours && now.minute() == minutes + 5))
    {

      digitalWrite(2, HIGH); //alarm sholat
      delay(1000);
      digitalWrite(2, LOW); //alarm sholat
      delay(1000);
      digitalWrite(2, HIGH); //alarm sholat
      delay(1000);
      digitalWrite(2, LOW); //alarm sholat
      delay(1000);

      dmd.clearScreen(true);
      dmd.selectFont(SystemFont5x7);
      dmd.drawString(15, 5, "TIME TO ADZAN SUBUH", 19, 0); //koordinat tampilan time to adzan
      delay(lamatimetoazan);

      //iqomah----------------------------------
      int langkah;
      int waktu_iqomah;
      int panjang_iqomah;
      int iqomah;
      dmd.clearScreen(true);
      switch (harin_e)
      {
        case 0: // your hand is on the sensor
          dmd.selectFont(SystemFont5x7);
          dmd.drawString(textimam_x, textimam_y, "IMAM SHOLAT SUBUH", 17, 0); //koordinat tampilan
          dmd.selectFont(SystemFont5x7);
          dmd.drawString(imam_x, imam_y, imam_subuh0, imam_subuh0pj, 0); //koordinat tampilan
          break;
        case 1: // your hand is close to the sensor

          dmd.selectFont(SystemFont5x7);
          dmd.drawString(textimam_x, textimam_y, "IMAM SHOLAT SUBUH", 17, 0); //koordinat tampilan
          dmd.selectFont(SystemFont5x7);
          dmd.drawString(imam_x, imam_y, imam_subuh1, imam_subuh1pj, 0); //koordinat tampilan

          break;
        case 2: // your hand is a few inches from the sensor

          dmd.selectFont(SystemFont5x7);
          dmd.drawString(textimam_x, textimam_y, "IMAM SHOLAT SUBUH", 17, 0); //koordinat tampilan
          dmd.selectFont(SystemFont5x7);
          dmd.drawString(imam_x, imam_y, imam_subuh2, imam_subuh2pj, 0); //koordinat tampilan
          break;
        case 3: // your hand is nowhere near the sensor
          dmd.selectFont(SystemFont5x7);
          dmd.drawString(textimam_x, textimam_y, "IMAM SHOLAT SUBUH", 17, 0); //koordinat tampilan
          dmd.selectFont(SystemFont5x7);
          dmd.drawString(imam_x, imam_y, imam_subuh3, imam_subuh3pj, 0); //koordinat tampilan
          break;
        case 4: // your hand is nowhere near the sensor
          dmd.selectFont(SystemFont5x7);
          dmd.drawString(textimam_x, textimam_y, "IMAM SHOLAT SUBUH", 17, 0); //koordinat tampilan
          dmd.selectFont(SystemFont5x7);
          dmd.drawString(imam_x, imam_y, imam_subuh4, imam_subuh4pj, 0); //koordinat tampilan
          break;
      }

      dmd.selectFont(SystemFont5x7);
      dmd.drawString(iqomah_x, iqomah_y, iqomahtext, 21, 0); //koordinat tampilan
      int tampil;
      int detik = 60, menit = lamaiqomah;
      for (detik = 60; detik >= 0; detik--)
      {
        delay(1000);
        String iqomah = Konversi(menit) + ":" + Konversi(detik);
        int panjang_iqomah = iqomah.length() + 1;
        char waktu_iqomah[panjang_iqomah];
        iqomah.toCharArray(waktu_iqomah, panjang_iqomah);
        dmd.drawString(129, 9, waktu_iqomah, panjang_iqomah, 0);
        if (detik <= 0)
        {
          detik = 60;
          menit = menit - 1;
        }
        /////buzer sholat////
        if (menit <= 0 && detik == 7)
        {
          digitalWrite(2, HIGH); //alarm sholat
        }
        if (menit <= 0 && detik == 6)
        {
          digitalWrite(2, LOW); //alarm sholat
        }
        if (menit <= 0 && detik == 5)
        {

          digitalWrite(2, HIGH); //alarm sholat
        }
        if (menit <= 0 && detik == 4)
        {
          digitalWrite(2, LOW); //alarm sholat
        }
        if (menit <= 0 && detik == 3)
        {
          digitalWrite(2, HIGH); //alarm sholat
        }
        if (menit <= 0 && detik == 2)
        {
          digitalWrite(2, LOW); //alarm sholat
        }
        ///////////////

        if (menit <= 0 && detik <= 1)
        {

          for (tampil = 0; tampil < 300; tampil++)
          {
            menit = 0;
            detik = 0;
            dmd.clearScreen(true);

            dmd.drawString(48, 0, "ITS TIME TO PRAY", 18, 0); //koordinat tampilan
            ///////////////////////
            now = rtc.now();
            String xjam = Konversi(now.hour()) + ":" + Konversi(now.minute()) + ":" + Konversi(now.second()); //tampilan jam
            int pjg = xjam.length() + 1;
            char sjam[pjg];
            xjam.toCharArray(sjam, pjg);
            dmd.drawString(57, 9, sjam, pjg, 0);

            /////////////////////
            delay(1000);
          }
        }
      }
    }
  }

  //jam adzan Dzuhur-------------------WAKTU DZUHUR MODIFIED--------------------------------------
  dmd.clearScreen(true);
  while (millis() - mulai < 85000)
  {
    ///////////////////////
    DateTime now = rtc.now();
    int harin_e = now.dayOfWeek();

    String xjam = Konversi(now.hour()) + ";" + Konversi(now.minute()) + ";" + Konversi(now.second()); //tampilan jam
    int pjg = xjam.length() + 1;
    char sjam[pjg];
    xjam.toCharArray(sjam, pjg);
    dmd.selectFont(angka_2);
    dmd.drawString(4, 5, sjam, pjg, 0);

    dmd.selectFont(SystemFont5x7);
    dmd.drawString(80, 5, (weekDay[now.dayOfWeek()]), 9, 0); //tampilan hari
    get_float_time_parts(times[2], hours, minutes);
    if ((now.hour() == hours && now.minute() == minutes && harin_e != 4) || (now.hour() == hours && now.minute() == minutes + 1 && harin_e != 4) || (now.hour() == hours && now.minute() == minutes + 2 && harin_e != 4) || (now.hour() == hours && now.minute() == minutes + 3 && harin_e != 4) || (now.hour() == hours && now.minute() == minutes + 4 && harin_e != 4) || (now.hour() == hours && now.minute() == minutes + 5 && harin_e != 4))
    {

      digitalWrite(2, HIGH); //alarm sholat
      delay(1000);
      digitalWrite(2, LOW); //alarm sholat
      delay(1000);
      digitalWrite(2, HIGH); //alarm sholat
      delay(1000);
      digitalWrite(2, LOW); //alarm sholat
      delay(1000);

      dmd.clearScreen(true);
      dmd.selectFont(SystemFont5x7);
      dmd.drawString(10, 5, "TIME TO ADZAN DZUHUR", 21, 0); //koordinat tampilan
      delay(lamatimetoazan);

      //iqomah----------------------------------
      int langkah;
      int waktu_iqomah;
      int panjang_iqomah;
      int iqomah;
      dmd.clearScreen(true);

      dmd.selectFont(SystemFont5x7);
      dmd.drawString(50, iqomah_y, iqomahtext, 6, 0); //koordinat tampilan

      int tampil;
      int detik = 60, menit = lamaiqomah;
      for (detik = 60; detik >= 0; detik--)
      {
        delay(1000);
        String iqomah = Konversi(menit) + ":" + Konversi(detik);
        int panjang_iqomah = iqomah.length() + 1;
        char waktu_iqomah[panjang_iqomah];
        iqomah.toCharArray(waktu_iqomah, panjang_iqomah);
        dmd.drawString(50, 9, waktu_iqomah, panjang_iqomah, 0);
        if (detik <= 0)
        {
          detik = 60;
          menit = menit - 1;
        }
        /////buzer sholat////
        if (menit <= 0 && detik == 7)
        {
          digitalWrite(2, HIGH); //alarm sholat
        }
        if (menit <= 0 && detik == 6)
        {
          digitalWrite(2, LOW); //alarm sholat
        }
        if (menit <= 0 && detik == 5)
        {
          digitalWrite(2, HIGH); //alarm sholat
        }
        if (menit <= 0 && detik == 4)
        {
          digitalWrite(2, LOW); //alarm sholat
        }
        if (menit <= 0 && detik == 3)
        {
          digitalWrite(2, HIGH); //alarm sholat
        }
        if (menit <= 0 && detik == 2)
        {
          digitalWrite(2, LOW); //alarm sholat
        }
        ///////////////

        if (menit <= 0 && detik <= 1)
        {
          for (tampil = 0; tampil < 300; tampil++)
          {
            menit = 0;
            detik = 0;
            dmd.clearScreen(true);

            dmd.drawString(48, 0, "ITS TIME TO PRAY", 17, 0); //koordinat tampilan
            ///////////////////////
            now = rtc.now();
            String xjam = Konversi(now.hour()) + ":" + Konversi(now.minute()) + ":" + Konversi(now.second()); //tampilan jam
            int pjg = xjam.length() + 1;
            char sjam[pjg];
            xjam.toCharArray(sjam, pjg);
            dmd.drawString(57, 9, sjam, pjg, 0);

            /////////////////////
            delay(1000);
          }
        }
      }
    }
  }

  //----------------------------------------------------------------------
  //jam hari Jum'at----JUM'AT--------JUM'AT--------JUM'AT----JUM'AT----JUM'AT----JUM'AT----JUM'AT----JUM'AT----JUM'AT------------------------------------------
  dmd.clearScreen( true );

  while (millis() - mulai < 90000) {
    ///////////////////////
    DateTime now = rtc.now();
    String hariku = Konversi(now.date()) + "/" + Konversi(now.month()) + "/" + Konversi(now.year()) + " "; //tampilan tanggal
    int dowo = hariku.length() + 1;
    char detikanku[dowo];
    hariku.toCharArray(detikanku, dowo);
    dmd.selectFont(SystemFont5x7);
    dmd.drawString(80, 5, detikanku, dowo, 0);
    String xjam = Konversi(now.hour()) + ";" + Konversi(now.minute()) + ";" + Konversi(now.second()); //tampilan jam
    int pjg = xjam.length() + 1;
    char sjam[pjg];
    xjam.toCharArray(sjam, pjg);
    dmd.selectFont(SystemFont5x7);
    dmd.drawString(5, 5, sjam, pjg, 0);




    /////////////////////
    int harin_e = now.dayOfWeek();
    get_float_time_parts(times[2], hours, minutes);
    if ((now.hour() == hours && now.minute() == minutes && harin_e == 4) || (now.hour() == hours && now.minute() == minutes + 1 && harin_e == 4) || (now.hour() == hours && now.minute() == minutes + 2 && harin_e == 4)
        || (now.hour() == hours && now.minute() == minutes + 3 && harin_e == 4)
        || (now.hour() == hours && now.minute() == minutes + 4 && harin_e == 4)
        || (now.hour() == hours && now.minute() == minutes + 5 && harin_e == 4)) {
      int tampil;
      dmd.clearScreen( true );
      for (tampil = 0; tampil < 1550 ; tampil++) {


        dmd.selectFont(SystemFont5x7);
        dmd.drawString(9, 0, "TIME TO KHOTBAH", 15, 0); //koordinat tampilan
        ///////////////////////
        now = rtc.now();
        String xjam = Konversi(now.hour()) + ":" + Konversi(now.minute()) + ":" + Konversi(now.second()); //tampilan jam
        int pjg = xjam.length() + 1;
        char sjam[pjg];
        xjam.toCharArray(sjam, pjg);

        dmd.drawString(62, 9, sjam, pjg, 0);

        /////////////////////
        delay (1000);

      }

    }


  }



  ////-----AKHIR CODING JIKA HARI JUM'AT-----



  //jam adzan Asar----------------------------------------------------------
  dmd.clearScreen(true);
  while (millis() - mulai < 95000)
  {
    ///////////////////////
    DateTime now = rtc.now();
    int harin_e = now.dayOfWeek();

    String xjam = Konversi(now.hour()) + ";" + Konversi(now.minute()) + ";" + Konversi(now.second()); //tampilan jam
    int pjg = xjam.length() + 1;
    char sjam[pjg];
    xjam.toCharArray(sjam, pjg);
    dmd.selectFont(angka_2);
    dmd.drawString(5, 5, sjam, pjg, 0);

    dmd.selectFont(SystemFont5x7);
    dmd.drawString(80, 5, (weekDay[now.dayOfWeek()]), 9, 0); //tampilan hari

    get_float_time_parts(times[3], hours, minutes);
    if ((now.hour() == hours && now.minute() == minutes) || (now.hour() == hours && now.minute() == minutes + 1) || (now.hour() == hours && now.minute() == minutes + 2) || (now.hour() == hours && now.minute() == minutes + 3) || (now.hour() == hours && now.minute() == minutes + 4) || (now.hour() == hours && now.minute() == minutes + 5))
    {

      digitalWrite(2, HIGH); //alarm sholat
      delay(1000);
      digitalWrite(2, LOW); //alarm sholat
      delay(1000);
      digitalWrite(2, HIGH); //alarm sholat
      delay(1000);
      digitalWrite(2, LOW); //alarm sholat
      delay(1000);

      dmd.clearScreen(true);
      dmd.selectFont(SystemFont5x7);
      dmd.drawString(10, 5, "TIME FOR ADZAN ASHAR", 20, 0); //koordinat tampilan
      delay(lamatimetoazan);

      //iqomah----------------------------------
      int langkah;
      int waktu_iqomah;
      int panjang_iqomah;
      int iqomah;

      dmd.clearScreen(true);
      

      dmd.selectFont(SystemFont5x7);
      dmd.drawString(50, iqomah_y, iqomahtext, 6, 0); //koordinat tampilan

      int tampil;
      int detik = 60, menit = lamaiqomah;
      for (detik = 60; detik >= 0; detik--)
      {
        delay(1000);
        String iqomah = Konversi(menit) + ":" + Konversi(detik);
        int panjang_iqomah = iqomah.length() + 1;
        char waktu_iqomah[panjang_iqomah];
        iqomah.toCharArray(waktu_iqomah, panjang_iqomah);
        dmd.drawString(50, 9, waktu_iqomah, panjang_iqomah, 0);
        if (detik <= 0)
        {
          detik = 60;
          menit = menit - 1;
        }
        /////buzer sholat////
        if (menit <= 0 && detik == 7)
        {
          digitalWrite(2, HIGH); //alarm sholat
        }
        if (menit <= 0 && detik == 6)
        {
          digitalWrite(2, LOW); //alarm sholat
        }
        if (menit <= 0 && detik == 5)
        {
          digitalWrite(2, HIGH); //alarm sholat
        }
        if (menit <= 0 && detik == 4)
        {
          digitalWrite(2, LOW); //alarm sholat
        }
        if (menit <= 0 && detik == 3)
        {
          digitalWrite(2, HIGH); //alarm sholat
        }
        if (menit <= 0 && detik == 2)
        {
          digitalWrite(2, LOW); //alarm sholat
        }
        ///////////////

        if (menit <= 0 && detik <= 1)
        {
          for (tampil = 0; tampil < 300; tampil++)
          {
            menit = 0;
            detik = 0;
            dmd.clearScreen(true);

            dmd.drawString(15, 0, "TIME TO PRAY", 13, 0); //koordinat tampilan
            ///////////////////////
            now = rtc.now();
            String xjam = Konversi(now.hour()) + ":" + Konversi(now.minute()) + ":" + Konversi(now.second()); //tampilan jam
            int pjg = xjam.length() + 1;
            char sjam[pjg];
            xjam.toCharArray(sjam, pjg);
            dmd.drawString(7, 9, sjam, pjg, 0);

            /////////////////////
            delay(1000);
          }
        }
      }
    }
  }

  //----------------------------------------------------------------------

  //jam adzan Maghrib----------------------------------------------------------
  dmd.clearScreen(true);
  while (millis() - mulai < 100000)
  {
    ///////////////////////
    DateTime now = rtc.now();
    int harin_e = now.dayOfWeek();
    String hariku = Konversi(now.date()) + "/" + Konversi(now.month()) + "/" + Konversi(now.year()) + " "; //tampilan tanggal
    int dowo = hariku.length() + 1;
    char detikanku[dowo];
    hariku.toCharArray(detikanku, dowo);
    dmd.selectFont(SystemFont5x7);
    dmd.drawString(80, 5, detikanku, dowo, 0);
    String xjam = Konversi(now.hour()) + ";" + Konversi(now.minute()) + ";" + Konversi(now.second()); //tampilan jam
    int pjg = xjam.length() + 1;
    char sjam[pjg];
    xjam.toCharArray(sjam, pjg);
    dmd.selectFont(angka_2);
    dmd.drawString(5, 5, sjam, pjg, 0);

    get_float_time_parts(times[4], hours, minutes);
    if ((now.hour() == hours && now.minute() == minutes) || (now.hour() == hours && now.minute() == minutes + 1) || (now.hour() == hours && now.minute() == minutes + 2) || (now.hour() == hours && now.minute() == minutes + 3) || (now.hour() == hours && now.minute() == minutes + 4) || (now.hour() == hours && now.minute() == minutes + 5))
    {

      digitalWrite(2, HIGH); //alarm sholat
      delay(1000);
      digitalWrite(2, LOW); //alarm sholat
      delay(1000);
      digitalWrite(2, HIGH); //alarm sholat
      delay(1000);
      digitalWrite(2, LOW); //alarm sholat
      delay(1000);

      dmd.clearScreen(true);
      dmd.selectFont(SystemFont5x7);
      dmd.drawString(10, 5, "TIME TO ADZAN MAGHRIB ", 21, 0); //koordinat tampilan
      
      delay(lamatimetoazan);

      //iqomah----------------------------------
      int langkah;
      int waktu_iqomah;
      int panjang_iqomah;
      int iqomah;

      dmd.clearScreen(true);
      switch (harin_e)
      {
        case 0: // your hand is on the sensor
          dmd.selectFont(SystemFont5x7);
          dmd.drawString(textimam_x, textimam_y, "IMAM SHOLAT MAGHRIB", 19, 0); //koordinat tampilan
          dmd.selectFont(SystemFont5x7);
          dmd.drawString(imam_x, imam_y, imam_magrib0, 21, 0); //koordinat tampilan
          break;
        case 1: // your hand is close to the sensor

          dmd.selectFont(SystemFont5x7);
          dmd.drawString(textimam_x, textimam_y, "IMAM SHOLAT MAGHRIB", 19, 0); //koordinat tampilan
          dmd.selectFont(SystemFont5x7);
          dmd.drawString(imam_x, imam_y, imam_magrib1, 21, 0); //koordinat tampilan

          break;
        case 2: // your hand is a few inches from the sensor

          dmd.selectFont(SystemFont5x7);
          dmd.drawString(textimam_x, textimam_y, "IMAM SHOLAT MAGHRIB", 19, 0); //koordinat tampilan
          dmd.selectFont(SystemFont5x7);
          dmd.drawString(imam_x, imam_y, imam_magrib2, 21, 0); //koordinat tampilan
          break;
        case 3: // your hand is nowhere near the sensor
          dmd.selectFont(SystemFont5x7);
          dmd.drawString(textimam_x, textimam_y, "IMAM SHOLAT MAGHRIB", 19, 0); //koordinat tampilan
          dmd.selectFont(SystemFont5x7);
          dmd.drawString(imam_x, imam_y, imam_magrib3, 21, 0); //koordinat tampilan
          break;
        case 4: // your hand is nowhere near the sensor
          dmd.selectFont(SystemFont5x7);
          dmd.drawString(textimam_x, textimam_y, "IMAM SHOLAT MAGHRIB", 19, 0); //koordinat tampilan
          dmd.selectFont(SystemFont5x7);
          dmd.drawString(imam_x, imam_y, imam_magrib4, 21, 0); //koordinat tampilan
          break;
      }

      dmd.selectFont(SystemFont5x7);
      dmd.drawString(iqomah_x, iqomah_y, iqomahtext, 6, 0); //koordinat tampilan

      int tampil;
      int detik = 60, menit = lamaiqomah;
      for (detik = 60; detik >= 0; detik--)
      {
        delay(1000);
        String iqomah = Konversi(menit) + ":" + Konversi(detik);
        int panjang_iqomah = iqomah.length() + 1;
        char waktu_iqomah[panjang_iqomah];
        iqomah.toCharArray(waktu_iqomah, panjang_iqomah);
        dmd.drawString(100, 5, waktu_iqomah, panjang_iqomah, 0);
        if (detik <= 0)
        {
          detik = 60;
          menit = menit - 1;
        }
        /////buzer sholat////
        if (menit <= 0 && detik == 7)
        {
          digitalWrite(2, HIGH); //alarm sholat
        }
        if (menit <= 0 && detik == 6)
        {
          digitalWrite(2, LOW); //alarm sholat
        }
        if (menit <= 0 && detik == 5)
        {
          digitalWrite(2, HIGH); //alarm sholat
        }
        if (menit <= 0 && detik == 4)
        {
          digitalWrite(2, LOW); //alarm sholat
        }
        if (menit <= 0 && detik == 3)
        {
          digitalWrite(2, HIGH); //alarm sholat
        }
        if (menit <= 0 && detik == 2)
        {

          digitalWrite(2, LOW); //alarm sholat
        }
        ///////////////
        if (menit <= 0 && detik <= 1)
        {
          for (tampil = 0; tampil < 300; tampil++)
          {
            menit = 0;
            detik = 0;
            dmd.clearScreen(true);

            dmd.drawString(15, 0, "TIME TO PRAY", 13, 0); //koordinat tampilan
            ///////////////////////
            now = rtc.now();
            String xjam = Konversi(now.hour()) + ":" + Konversi(now.minute()) + ":" + Konversi(now.second()); //tampilan jam
            int pjg = xjam.length() + 1;
            char sjam[pjg];
            xjam.toCharArray(sjam, pjg);
            dmd.drawString(7, 9, sjam, pjg, 0);

            /////////////////////
            delay(1000);
          }
        }
      }
    }
  }

  //jam adzan isya----------------------------------------------------------
  dmd.clearScreen(true);
  while (millis() - mulai < 105000)
  {
    ///////////////////////
    DateTime now = rtc.now();
    int harin_e = now.dayOfWeek();
    String xjam = Konversi(now.hour()) + ";" + Konversi(now.minute()) + ";" + Konversi(now.second()); //tampilan jam
    int pjg = xjam.length() + 1;
    char sjam[pjg];
    xjam.toCharArray(sjam, pjg);
    dmd.selectFont(angka_2);
    dmd.drawString(5, 5, sjam, pjg, 0);

    dmd.selectFont(SystemFont5x7);
    dmd.drawString(80, 5, (weekDay[now.dayOfWeek()]), 7, 0); //tampilan hari

    get_float_time_parts(times[6], hours, minutes);
    if ((now.hour() == hours && now.minute() == minutes) || (now.hour() == hours && now.minute() == minutes + 1) || (now.hour() == hours && now.minute() == minutes + 2) || (now.hour() == hours && now.minute() == minutes + 3) || (now.hour() == hours && now.minute() == minutes + 4) || (now.hour() == hours && now.minute() == minutes + 5))
    {

      digitalWrite(2, HIGH); //alarm sholat
      delay(1000);
      digitalWrite(2, LOW); //alarm sholat
      delay(1000);
      digitalWrite(2, HIGH); //alarm sholat
      delay(1000);
      digitalWrite(2, LOW); //alarm sholat
      delay(1000);

      dmd.clearScreen(true);
      dmd.selectFont(SystemFont5x7);
      dmd.drawString(10, 5, "TIME TO ADZAN ISYA", 20, 0); //koordinat tampilan
     
      delay(lamatimetoazan);
      //iqomah----------------------------------
      int langkah;
      int waktu_iqomah;
      int panjang_iqomah;
      int iqomah;
      dmd.clearScreen(true);
      switch (harin_e)
      {
        case 0: // your hand is on the sensor
          dmd.selectFont(SystemFont5x7);
          dmd.drawString(textimam_x, textimam_y, "IMAM SHOLAT ISYA", 17, 0); //koordinat tampilan
          dmd.selectFont(SystemFont5x7);
          dmd.drawString(imam_x, imam_y, imam_isya0, 21, 0); //koordinat tampilan
          break;
        case 1: // your hand is close to the sensor

          dmd.selectFont(SystemFont5x7);
          dmd.drawString(textimam_x, textimam_y, "IMAM SHOLAT ISYA", 17, 0); //koordinat tampilan
          dmd.selectFont(SystemFont5x7);
          dmd.drawString(imam_x, imam_y, imam_isya1, 21, 0); //koordinat tampilan

          break;
        case 2: // your hand is a few inches from the sensor

          dmd.selectFont(SystemFont5x7);
          dmd.drawString(textimam_x, textimam_y, "IMAM SHOLAT ISYA", 17, 0); //koordinat tampilan
          dmd.selectFont(SystemFont5x7);
          dmd.drawString(imam_x, imam_y, imam_isya2, 21, 0); //koordinat tampilan
          break;
        case 3: // your hand is nowhere near the sensor
          dmd.selectFont(SystemFont5x7);
          dmd.drawString(textimam_x, textimam_y, "IMAM SHOLAT ISYA", 17, 0); //koordinat tampilan
          dmd.selectFont(SystemFont5x7);
          dmd.drawString(imam_x, imam_y, imam_isya3, 21, 0); //koordinat tampilan
          break;
        case 4 : // your hand is nowhere near the sensor
          dmd.selectFont(SystemFont5x7);
          dmd.drawString(textimam_x, textimam_y, "IMAM SHOLAT ISYA", 17, 0); //koordinat tampilan
          dmd.selectFont(SystemFont5x7);
          dmd.drawString(imam_x, imam_y, imam_isya4, 21, 0); //koordinat tampilan

      }

      dmd.selectFont(SystemFont5x7);
      dmd.drawString(iqomah_x, iqomah_y, iqomahtext, 21, 0); //koordinat tampilan
      int tampil;
      int detik = 60, menit = lamaiqomah;
      for (detik = 60; detik >= 0; detik--)
      {
        delay(1000);
        String iqomah = Konversi(menit) + ":" + Konversi(detik);
        int panjang_iqomah = iqomah.length() + 1;
        char waktu_iqomah[panjang_iqomah];
        iqomah.toCharArray(waktu_iqomah, panjang_iqomah);
        dmd.drawString(100, 5, waktu_iqomah, panjang_iqomah, 0);
        if (detik <= 0)
        {
          detik = 60;
          menit = menit - 1;
        }
        /////buzer sholat////
        if (menit <= 0 && detik == 8)
        {
          digitalWrite(2, HIGH);
        }
        if (menit <= 0 && detik == 8)
        {
          digitalWrite(2, HIGH);
        }
        if (menit <= 0 && detik == 7)
        {
          digitalWrite(2, HIGH); //alarm sholat
        }
        if (menit <= 0 && detik == 6)
        {
          digitalWrite(2, LOW); //alarm sholat
        }
        if (menit <= 0 && detik == 5)
        {
          digitalWrite(2, HIGH); //alarm sholat
        }
        if (menit <= 0 && detik == 4)
        {
          digitalWrite(2, LOW); //alarm sholat
        }
        if (menit <= 0 && detik == 3)
        {
          digitalWrite(2, HIGH); //alarm sholat
        }
        if (menit <= 0 && detik == 2)
        {
          digitalWrite(2, LOW); //alarm sholat
        }

        if (menit <= 0 && detik <= 1)
        {
          for (tampil = 0; tampil < 300; tampil++)
          {
            menit = 0;
            detik = 0;
            dmd.clearScreen(true);

            dmd.drawString(63, 0, "TIME TO PRAY", 13, 0); //koordinat tampilan

            now = rtc.now();
            String xjam = Konversi(now.hour()) + ":" + Konversi(now.minute()) + ":" + Konversi(now.second()); //tampilan jam
            int pjg = xjam.length() + 1;
            char sjam[pjg];
            xjam.toCharArray(sjam, pjg);
            dmd.drawString(57, 9, sjam, pjg, 0);

            /////////////////////
            delay(1000);
          }
        }
      }
    }
  }
}

String Konversi(int sInput)
{
  if (sInput < 10)
  {
    return "0" + String(sInput);
  }
  else
  {
    return String(sInput);
  }
}