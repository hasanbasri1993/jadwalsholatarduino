

////////tutorial lebih lengkap kunjungi : www.rokhmad.com  //////

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
char weekDay[][7] = {
    "SENIN",
    "SELASA",
    "RABU",
    "KAMIS",
    "JUM'AT",
    "SABTU",
    "AHAD",
    "SENIN",
}; // array hari, dihitung mulai dari senin, hari senin angka nya =0,
int langkah;
int lama_tampilnya;
boolean Waduh_Setting = false;
///////////////////////////////// Mendifinisikan Pin Tombol ////////////////////////////////////////
#define Jam_bUP A1    // Tombol setting sesuaikan dengan PIN di arduino anda mana  klo ini terhubung dengan PIN 3 Digital
#define tombol_bOK A2 // Tombol OK sesuaikan dengan PIN di arduino anda mana  klo ini terhubung dengan PIN 5 Digital
#define Jam_bDN A3    //tombol setting   sesuaikan dengan PIN di arduino anda mana  klo ini terhubung dengan PIN 4 Digital

///////////////////koding ini untuk setting rtc sesuai dengan laptop//// /////
//year, month, date, hour, min, sec and week-day(starts from 0 and goes to 6)
//writing any non-existent time-data may interfere with normal operation of the RTC.
//Take care of week-day also.
//DateTime dt(2017, 5, 4, 15, 11, 0, 4);

/*
 * PIN YANG DISAMBUNG
 * Arduino              P-10
 * -D6          ->      -A
 * -D7          ->      -B
 * -D8          ->      -SCLK
 * -D9          ->      -OE
 * -D11         ->      -DATA
 * -D13         ->      -CLK
 * -GND         ->      -GND
 */

DMD dmd(5, 1); //untuk mengatur jumlah panel yang kamu pakai
long start;
double lintang = -7.0667; //Latitude
double bujur = 110.4;     //Longitude
int Z = 7;                //Zona Waktu GMT WIB biasanya 7

double times[sizeof(TimeName) / sizeof(char *)];

void ScanDMD()
{
  dmd.scanDisplayBySPI();
}

void setup()
{

  pinMode(2, OUTPUT); //----pin 2 untuk alarm adzan maghrib ---hubungkan dengan buzzer

  langkah = 0;
  rtc.begin();
  //rtc.setDateTime(dt); //Adjust date-time as defined 'dt' above (ini untuk setting jam)
  Wire.begin();
  Timer1.initialize(500);
  Timer1.attachInterrupt(ScanDMD);
  dmd.clearScreen(true);

  pinMode(tombol_bOK, INPUT_PULLUP); // Mode Pin Sebagai Input dengan Pull Up Internal
  pinMode(Jam_bUP, INPUT_PULLUP);    // Mode Pin Sebagai Input dengan Pull Up Internal
  pinMode(Jam_bDN, INPUT_PULLUP);    // Mode Pin Sebagai Input dengan Pull Up Internal

  attachInterrupt(0, Setting, FALLING);
}

////////////////////////////////// Fungsi Looping/Pengulangan ///////////////////////////////////////
void loop()
{
  dmd.setKecerahan(5);
  if (Waduh_Setting)
  {

    MenuSetting(); // Jika Tombol OK/Interrupt ditekan maka masuk menu setting Manual
  }
  mulai();
  start = millis();
}

/////////////////////////////////// Interrupt Button Setting ////////////////////////////////////////
void Setting()
{
  Waduh_Setting = true; // Aktifkan Menu Setting
}

////////////////////////////////////// Fungsi Menu Setting //////////
void MenuSetting()
{
  DateTime now = rtc.now();
  String Teks;
  int tgl = now.date();       //Tanggal
  int bln = now.month();      //Bulan
  int thn = now.year();       //Tahun
  int hours, minutes;         //jam dan menit jawdwal sholat
  int jame = now.hour();      //jam
  int menit_e = now.minute(); //menit
  int detik_e = now.second(); //detik
  int harin_e = harin_e;      //hari
  detachInterrupt(0);         // Matikan interrupt "0"
  delay(500);

  /////////////////////////////////////// Setting Jam & Menit ///////

  int kasus = 0;
setTimes: // Setting Times (Jam & Menit)
  if (digitalRead(Jam_bUP) == LOW)
  { // Tombol Setting Menit
    delay(10);
    menit_e++; // Delay Tombol & Tambah Nilai Menit
    if (menit_e >= 60)
    {
      menit_e = 0; // Batas Nilai Menit Lebih dari 59 Kembali ke 0
    }
    while (digitalRead(Jam_bUP) == LOW)
    {
    } // Kunci Tombol Setting Menit Sampai Lepas Tombol
  }
  if (digitalRead(Jam_bDN) == LOW)
  { // Tombol Setting Menit
    delay(10);
    jame++; // Delay Tombol & Tambah Nilai Jam
    if (jame >= 24)
    {
      jame = 0; // Batas Nilai Jam Lebih dari 23 Kembali ke 0
    }
    while (digitalRead(Jam_bDN) == LOW)
    {
    } // Kunci Tombol Setting Jam Sampai Lepas Tombol
  }

setTgl: // Setting tanggal
  if (digitalRead(Jam_bUP) == LOW)
  { // Tombol Setting tanggal
    delay(10);
    tgl++;
    if (tgl >= 32)
    {
      tgl = 1; // Batas Nilai tgl Lebih dari 31 Kembali ke 1
    }
    while (digitalRead(Jam_bUP) == LOW)
    {
    } // Kunci Tombol Setting tgl Sampai Lepas Tombol
  }
  if (digitalRead(Jam_bDN) == LOW)
  { // Tombol Setting bln
    delay(10);
    bln++; // Delay Tombol & Tambah Nilai bln
    if (bln >= 13)
    {
      bln = 1; // Batas Nilai bln Lebih dari 12 Kembali ke 1
    }
    while (digitalRead(Jam_bDN) == LOW)
    {
    } // Kunci Tombol Setting blnSampai Lepas Tombol
  }

  // ///////=======setting thn==

setThn: // Setting thn
  if (digitalRead(Jam_bUP) == LOW)
  { // Tombol Setting thn
    delay(10);
    thn++; // Delay Tombol & Tambah Nilai thn
    if (thn >= 9999)
    {
      thn = 0; // Batas Nilai Menit Lebih dari 9999 Kembali ke 0
    }
    while (digitalRead(Jam_bUP) == LOW)
    {
    } // Kunci Tombol Setting thn Sampai Lepas Tombol
  }
  if (digitalRead(Jam_bDN) == LOW)
  { // Tombol Setting thn
    delay(10);
    thn--; // Delay Tombol & Tambah Nilai thn
    if (thn <= 0)
    {
      thn = 0; // Batas Nilai kurang dari 0 Kembali ke 0
    }
    while (digitalRead(Jam_bDN) == LOW)
    {
    } // Kunci Tombol Setting thn Sampai Lepas Tombol
  }

  // =======setting Hari======
  //hitungan hari mulai dari 0,1,2,3,4,5,6 = senin, selasa, rabu, kamis, jumat, sabtu, minggu

setHari: // Setting Times hari
  if (digitalRead(Jam_bUP) == LOW)
  { // Tombol Setting hari
    harin_e = harin_e;
    delay(10);
    harin_e++; // Delay Tombol & Tambah Nilai hari,
    if (harin_e >= 7)
    {
      harin_e = 0; // Batas Nilai Menit Lebih dari 6 Kembali ke 0
    }
    while (digitalRead(Jam_bUP) == LOW)
    {
    } // Kunci Tombol Setting hariSampai Lepas Tombol
  }
  if (digitalRead(Jam_bDN) == LOW)
  { // Tombol Setting hari
    harin_e = harin_e;
    delay(10);
    harin_e--; // Delay Tombol & Tambah Nilai hari
    if (harin_e <= 0)
    {
      harin_e = 0; // Batas Nilai kurang dari 0 Kembali ke 0
    }
    while (digitalRead(Jam_bDN) == LOW)
    {
    } // Kunci Tombol Setting hari Sampai Lepas Tombol
  }

  ///============Tombol OKE =======================

  if (digitalRead(tombol_bOK) == LOW)
  { // Tombol Selesai Setting

    kasus = kasus + 1;
    dmd.clearScreen(true); // Hapus Tampilan P10
    DateTime dt(thn, bln, tgl, jame, menit_e, now.second(), harin_e);
    // DateTime dt(2017, 5, 4, 15, 11, 0, 4);
    rtc.setDateTime(dt); //Adjust date-time as defined 'dt' above
    while (digitalRead(tombol_bOK) == LOW)
    {
    }
    delay(500);

    //  ============================================kasus-kasus===================
  }
  if (kasus == 1)
  { //kasus seting jam

    dmd.clearScreen(true);
    dmd.selectFont(SystemFont5x7);
    dmd.drawString(0, 0, "set-J", 5, 0);
    String xjam = Konversi(jame) + ":" + Konversi(menit_e);
    int pjg = xjam.length() + 1;
    char sjam[pjg];
    xjam.toCharArray(sjam, pjg);
    dmd.drawString(0, 9, sjam, pjg, 0);
    delay(100);
    goto setTimes; //lari ke fungsi seting jam
  }
  if (kasus == 2)
  { //kasus seting tanggal

    dmd.clearScreen(true);
    dmd.selectFont(SystemFont5x7);
    dmd.drawString(0, 0, "Tgl", 3, 0);
    String xjam = Konversi(tgl) + "/" + Konversi(bln);
    int pjg = xjam.length() + 1;
    char sjam[pjg];
    xjam.toCharArray(sjam, pjg);
    dmd.drawString(0, 9, sjam, pjg, 0);
    delay(100);
    goto setTgl; //lari ke fungsi setting tanggal
  }
  if (kasus == 3)
  { ///kasus seting tahun

    dmd.clearScreen(true);
    dmd.selectFont(SystemFont5x7);
    dmd.drawString(0, 0, "Thn", 3, 0);
    String xjam = Konversi(thn);
    int pjg = xjam.length() + 1;
    char sjam[pjg];
    xjam.toCharArray(sjam, pjg);
    dmd.drawString(0, 9, sjam, pjg, 0);
    delay(100);
    goto setThn; //lari ke fungsi setting tahun
  }

  if (kasus == 4)
  { //kasus seting hari

    dmd.clearScreen(true);
    dmd.selectFont(SystemFont5x7);
    dmd.drawString(0, 0, "Hari", 4, 0);
    String xjam = Konversi(harin_e);
    if (harin_e == 0)
    {
      xjam = "SEN";
      int pjg = xjam.length() + 1;
      char sjam[pjg];
      xjam.toCharArray(sjam, pjg);
      dmd.drawString(0, 9, sjam, pjg, 0);
    }
    if (harin_e == 1)
    {
      xjam = "SEL";

      int pjg = xjam.length() + 1;
      char sjam[pjg];
      xjam.toCharArray(sjam, pjg);
      dmd.drawString(0, 9, sjam, pjg, 0);
    }
    if (harin_e == 2)
    {
      xjam = "RAB";

      int pjg = xjam.length() + 1;
      char sjam[pjg];
      xjam.toCharArray(sjam, pjg);
      dmd.drawString(0, 9, sjam, pjg, 0);
    }
    if (harin_e == 3)
    {
      xjam = "KAM";

      int pjg = xjam.length() + 1;
      char sjam[pjg];
      xjam.toCharArray(sjam, pjg);
      dmd.drawString(0, 9, sjam, pjg, 0);
    }
    if (harin_e == 4)
    {
      xjam = "JUM";

      int pjg = xjam.length() + 1;
      char sjam[pjg];
      xjam.toCharArray(sjam, pjg);
      dmd.drawString(0, 9, sjam, pjg, 0);
    }
    if (harin_e == 5)
    {
      xjam = "SAB";

      int pjg = xjam.length() + 1;
      char sjam[pjg];
      xjam.toCharArray(sjam, pjg);
      dmd.drawString(0, 9, sjam, pjg, 0);
    }
    if (harin_e == 6)
    {
      xjam = "MIN";

      int pjg = xjam.length() + 1;
      char sjam[pjg];
      xjam.toCharArray(sjam, pjg);
      dmd.drawString(0, 9, sjam, pjg, 0);
    }
    delay(100);
    goto setHari; //lari ke seting hari
  }

  if (kasus == 5)
  { //kasus wis Rampung
    dmd.clearScreen(true);
    goto endSetting;
    delay(500);
  }

endSetting:                             // Label End Setting
  attachInterrupt(0, Setting, FALLING); // Interrupt Tombol OK
  Waduh_Setting = false;

  kasus = 0;
}

//////////////////////==========================Program Jam==============

int menite = 1; ///waktu iqomah lamanya
void mulai()
{
  dmd.clearScreen(true);
  long mulai = millis();
  while (millis() - start < 10000)
  {

    DateTime now = rtc.now();
    String Teks;

    ///================pengaturan koordinat-gunakan gps android untuk menentukan posisi anda=================

    set_calc_method(ISNA);  //Methode perhitungan mengikuti ISNA
    set_asr_method(Shafii); //Pendapat Imam Syafi'i
    set_high_lats_adjust_method(AngleBased);
    set_fajr_angle(20); //Sudut Waktu Subuh
    set_isha_angle(18); ////Sudut Waktu Isya

    int tgl = now.date();  //Tanggal
    int bln = now.month(); //Bulan
    int thn = now.year();  //Tahun
    int hours, minutes;    //jam sholat dan  menit sholat

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
    dmd.drawChar(14, 4, ':', GRAPHICS_NOR); // clock colon overlay on
    delay(100);

    dmd.drawChar(14, 4, ':', GRAPHICS_OR); // clock colon overlay off
    delay(100);

    //showperjamsholat(int urutansholatke, String Teks, int x, int y)
    showperjamsholat(0, "Subuh", 24, 0);
    showperjamsholat(2, "Dzuhur", 24, 0);
    showperjamsholat(3, "Ashar", 24, 0);
    showperjamsholat(4, "Magrib", 24, 0);
    showperjamsholat(6, "Isya'", 24, 0);
    showperjamsholat(8, "Imsak'", 24, 0);

    ///TOMBOL SET JAM//
    if (digitalRead(tombol_bOK) == LOW)
    {
      MenuSetting(); // Jika Tombol OK/Interrupt ditekan maka masuk menu setting Manual
    }
  }

  ////runing teks jam diatas----//

  dmd.clearScreen(true);
  while (millis() - start < 20000)
  {
    DateTime now = rtc.now();
    String Teks;
    delay(100);
    detachInterrupt(0); // Matikan interrupt "0"

    ////   // Tampilkan hari

    dmd.selectFont(SystemFont5x7);
    String hariku = "MASJID MUSTAQIM      " + Konversi(now.hour()) + ":" + Konversi(now.minute());
    int dowo = hariku.length() + 1;
    char detikanku[dowo];
    hariku.toCharArray(detikanku, dowo);
    dmd.drawString(4, 0, detikanku, dowo, 0);

    Teks = "TANGGAL " + Konversi(now.date()) + "-" + Konversi(now.month()) + "-" + Konversi(now.year()) + " MASJID MUSTAQIM MASARAN, SRAGEN-SHOLAT BERJAMAAH LEBIH UTAMA DARIPADA SENDIRI-HP MOHON DI NON AKTIFKAN"; ///tampilkan hari
    int kecepatan;
    kecepatan = 30; //kecepatan runing teks
    int pj = Teks.length() + 1;
    char tampil[pj];
    Teks.toCharArray(tampil, pj);
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

        ///TOMBOL SET JAM//
        if (digitalRead(tombol_bOK) == LOW)
        {
          MenuSetting(); // Jika Tombol OK/Interrupt ditekan maka masuk menu setting Manual
        }
      }
    }
  }
}

void showperjamsholat(int urutansholatke, String Teks, int x, int y)
{
  int hours, minutes; //jam sholat dan  menit sholat
  if (urutansholatke = 8)
  {
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
  }
  else
  {
    get_float_time_parts(times[urutansholatke], hours, minutes);
  }

  //Teks = "Subuh   "; //baris atas
  char pj = Teks.length() + 1;
  char tampil[pj];
  Teks.toCharArray(tampil, pj);
  dmd.selectFont(SystemFont5x7);
  dmd.drawString(42, 0, tampil, pj, 0);             //koordinat tampilan
  Teks = Konversi(hours) + ":" + Konversi(minutes); //baris kedua
  pj = Teks.length() + 1;
  tampil[pj];
  Teks.toCharArray(tampil, pj);
  dmd.selectFont(SystemFont5x7);
  dmd.drawString(x, y, tampil, pj, 0); //koordinat tampilan

  dmd.drawChar(14, 4, ':', GRAPHICS_NOR); // clock colon overlay on
  delay(1000);
  dmd.drawChar(14, 4, ':', GRAPHICS_OR); // clock colon overlay off
  delay(1000);

  /////////
}

void cekjamsholat(int isjumat)
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
  ///=============================PROGRAM ADZAN DAN IQOMAH===================
  //jam adzan Subuh----------------------------------------------------------
  int hours;
  int minutes;
  dmd.clearScreen(true);
  while (millis() - start < 80000)
  {
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
    dmd.selectFont(angka_2);
    dmd.drawString(5, 5, sjam, pjg, 0);
    ///////////////interupsi tombol menu-----------

    if (digitalRead(tombol_bOK) == LOW)
    {
      MenuSetting(); // Jika Tombol OK/Interrupt ditekan maka masuk menu setting Manual
    }

    /////////////////////

    get_float_time_parts(times[0], hours, minutes);

    switch (isjumat)
    {
    case 1:
      if ((now.hour() == hours && now.minute() == minutes && harin_e != 4) || (now.hour() == hours && now.minute() == minutes + 1 && harin_e != 4) || (now.hour() == hours && now.minute() == minutes + 2 && harin_e != 4) || (now.hour() == hours && now.minute() == minutes + 3 && harin_e != 4) || (now.hour() == hours && now.minute() == minutes + 4 && harin_e != 4) || (now.hour() == hours && now.minute() == minutes + 5 && harin_e != 4))
        break;
    case 2:
      if ((now.hour() == hours && now.minute() == minutes && harin_e == 4) || (now.hour() == hours && now.minute() == minutes + 1 && harin_e == 4) || (now.hour() == hours && now.minute() == minutes + 2 && harin_e == 4) || (now.hour() == hours && now.minute() == minutes + 3 && harin_e == 4) || (now.hour() == hours && now.minute() == minutes + 4 && harin_e == 4) || (now.hour() == hours && now.minute() == minutes + 5 && harin_e == 4))
        break;
    default:
      if ((now.hour() == hours && now.minute() == minutes) || (now.hour() == hours && now.minute() == minutes + 1) || (now.hour() == hours && now.minute() == minutes + 2) || (now.hour() == hours && now.minute() == minutes + 3) || (now.hour() == hours && now.minute() == minutes + 4) || (now.hour() == hours && now.minute() == minutes + 5))
        break;
    }

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
      dmd.drawString(10, 5, "Waktunya Adzan ", 15, 0); //koordinat tampilan
      dmd.selectFont(SystemFont5x7);
      dmd.drawString(110, 5, "Subuh", 5, 0); //koordinat tampilan
      delay(150000);

      //iqomah----------------------------------
      int langkah;
      int waktu_iqomah;
      int panjang_iqomah;
      int iqomah;
      dmd.clearScreen(true);

      dmd.selectFont(SystemFont5x7);
      dmd.clearScreen(true);
      dmd.drawString(15, 5, "IQOMAH", 6, 0); //koordinat tampilan
      int tampil;
      int detik = 60, menit = 10;
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

            dmd.drawString(15, 0, "SHOLAT", 6, 0); //koordinat tampilan
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
}
///konversi angka agar ada nol didepannya jika diawah 10

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
