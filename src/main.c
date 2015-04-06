#include "main.h"
#include <pebble.h>
#include "PDUtils.h"

//Pebble KEYS
enum TripTimeKeys {
	LOCAL_NAME_KEY = 0,       // TUPLE_CSTRING
	LOCAL_TZ_KEY = 1, 		// TUPLE_INT
	LOCAL_TZNAME_KEY = 2,     // TUPLE_CSTRING
	DUAL_NAME_KEY = 3,  		// TUPLE_CSTRING
	DUAL_TZ_KEY = 4, 			// TUPLE_INT
	DUAL_TZNAME_KEY = 5,  	// TUPLE_CSTRING
	LOCAL_TEMP_KEY = 6,     // TUPLE_CSTRING
	LOCAL_ICON_KEY = 7,     // TUPLE_INT
	DUAL_TEMP_KEY = 8,     // TUPLE_CSTRING
	DUAL_ICON_KEY = 9,     // TUPLE_INT
	DUAL_NAME2_KEY = 10, //TUPLE_CSTRING
	DUAL_TZ2_KEY = 11, //TUPLE_INT
	DUAL_TEMP2_KEY = 12, //TUPLE_CSTRING
	DUAL_ICON2_KEY = 13, //TUPLE_INT
	DUAL_NAME3_KEY = 14, //TUPLE_CSTRING
	DUAL_TZ3_KEY = 15, //TUPLE_INT
	DUAL_TEMP3_KEY = 16, //TUPLE_CSTRING
	DUAL_ICON3_KEY = 17, //TUPLE_INT
};

//Bitmaps
static const uint32_t WEATHER_ICONS[] = {
  RESOURCE_ID_ICON_CLEAR_DAY,
  RESOURCE_ID_ICON_CLEAR_NIGHT,
  RESOURCE_ID_ICON_WIND,
  RESOURCE_ID_ICON_COLD,
  RESOURCE_ID_ICON_HOT,
  RESOURCE_ID_ICON_PARTLY_CLOUDY_DAY,
  RESOURCE_ID_ICON_PARTLY_CLOUDY_NIGHT,
  RESOURCE_ID_ICON_FOG,
  RESOURCE_ID_ICON_RAIN,
  RESOURCE_ID_ICON_SNOW,
  RESOURCE_ID_ICON_SLEET,
  RESOURCE_ID_ICON_SNOW_SLEET,
  RESOURCE_ID_ICON_RAIN_SLEET,
  RESOURCE_ID_ICON_RAIN_SNOW,
  RESOURCE_ID_ICON_CLOUDY,
  RESOURCE_ID_ICON_THUNDER,
  RESOURCE_ID_ICON_NA,
  RESOURCE_ID_ICON_DRIZZLE,
  RESOURCE_ID_TakeOff,
  RESOURCE_ID_Landing,
}; 
	
//Variables
	static char localtime_text[] = "00:00";
	static char localweekday_text[] = "XXXXXXXXXXXX";
	static char localmonth_text[] = "XXXXXXXXXXXXX";
	static char localdate_text[] = "XXXXXXXXXXXXXXXXXXXXXXXXX";

	static char dualtime_text[] = "00:00";
	static char dualweekday_text[] = "XXXXXXXXXXXX";
	static char dualmonth_text[] = "XXXXXXXXXXXXX";

	static char dualtime2_text[] = "00:00";
	static char dualweekday2_text[] = "XXXXXXXXXXXX";
	static char dualmonth2_text[] = "XXXXXXXXXXXXX";

	static char dualtime3_text[] = "00:00";
	static char dualweekday3_text[] = "XXXXXXXXXXXX";
	static char dualmonth3_text[] = "XXXXXXXXXXXXX";

	int localtz=0;
	int dualtz=0;
	int dualtz2=0;
	int dualtz3=0;
	int timediff=0; //dualtz-localtz
	int timediff2=0; //dualtz2-localtz
	int timediff3=0; //dualtz3-localtz

	char localname[] = "XXXXXXXXXXXXXXX";
	char dualname[] = "XXXXXXXXXXXXXXX";
	char dualname2[] = "XXXXXXXXXXXXXXX";
	char dualname3[] = "XXXXXXXXXXXXXXX";

	char LocalTZName[]  = "     ";
	char DualTZName[]  = "     ";

	char strLocalTemp[] = "    ";
    char strDualTemp[] = "    ";
	char strDualTemp2[] = "    ";
	char strDualTemp3[] = "    ";

	char strLocalAMPMInd[]  = "  ";
	char strDualAMPMInd[]  = "  ";

	static AppTimer *timer;
	uint32_t timeout_ms = 1800000; //30min (1min = 60000)

	// Setup messaging
	const int inbound_size = 512;
	const int outbound_size = 512;
	
// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
InverterLayer *inv_layer;
static Window *s_window;
static GFont s_res_gothic_14;
static GFont s_res_bitham_42_light;
static GFont s_res_bitham_30_black;
static GFont s_res_roboto_21_condensed;
static GBitmap *s_res_takeoff;
static GBitmap *s_res_landing;
static GBitmap *s_res_landing2;
static GBitmap *s_res_landing3;
static TextLayer *LocalArea;
static TextLayer *DualArea;

static TextLayer *LocalTZ;
static TextLayer *LocalTemp;
static TextLayer *LocalTime;
static TextLayer *LocalAMPMInd;
static TextLayer *LocalDate;
static TextLayer *LocalDay;
static TextLayer *DualTime;
//static TextLayer *DualAMPMInd;
static TextLayer *DualDate;
static TextLayer *DualDay;
static TextLayer *DualTemp;
//Timezone 2
static TextLayer *DualArea2;
static TextLayer *DualTime2;
//static TextLayer *DualAMPMInd2;
static TextLayer *DualDate2;
static TextLayer *DualDay2;
static TextLayer *DualTemp2;
static TextLayer *DualTZName2;
static BitmapLayer *Dual_img2;

static BitmapLayer *Local_img;
static BitmapLayer *Dual_img;
static TextLayer *s_textlayer_1;
static TextLayer *s_textlayer_2;

//Timezone 3
static TextLayer *DualArea3;
static TextLayer *DualTime3;
//static TextLayer *DualAMPMInd2;
static TextLayer *DualDate3;
static TextLayer *DualDay3;
static TextLayer *DualTemp3;
static TextLayer *DualTZName3;
static BitmapLayer *Dual_img3;


static void initialise_ui(void) {
  s_window = window_create();
  window_set_background_color(s_window, GColorBlack);
  window_set_fullscreen(s_window, true);
  
  s_res_gothic_14 = fonts_get_system_font(FONT_KEY_GOTHIC_14);
  s_res_bitham_42_light = fonts_get_system_font(FONT_KEY_BITHAM_42_LIGHT);
  s_res_bitham_30_black = fonts_get_system_font(FONT_KEY_BITHAM_30_BLACK);
  s_res_roboto_21_condensed = fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21);
	
////////////////////
// LOCAL TIMEZONE //
////////////////////
	
//AREA
  LocalArea = text_layer_create(GRect(3, 3, 137, 51));
  text_layer_set_font(LocalArea, s_res_gothic_14);
  layer_add_child(window_get_root_layer(s_window), (Layer *)LocalArea);
	
//CITY NAME
  s_textlayer_1 = text_layer_create(GRect(28, 2, 111, 17));
  text_layer_set_background_color(s_textlayer_1, GColorClear);
  text_layer_set_text(s_textlayer_1, localname);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_1);
    
//DATE
  LocalDate = text_layer_create(GRect(28, 14, 110, 16));
  text_layer_set_background_color(LocalDate, GColorClear);
  text_layer_set_text(LocalDate, localmonth_text);
  text_layer_set_font(LocalDate, s_res_gothic_14);
  layer_add_child(window_get_root_layer(s_window), (Layer *)LocalDate);
	
//TIME
  LocalTime = text_layer_create(GRect(45, 23, 93, 42));
  text_layer_set_background_color(LocalTime, GColorClear);
  text_layer_set_text(LocalTime, localtime_text);
  text_layer_set_text_alignment(LocalTime, GTextAlignmentRight);
  text_layer_set_font(LocalTime, s_res_bitham_30_black);
  layer_add_child(window_get_root_layer(s_window), (Layer *)LocalTime);
	
//WEATHER//

//ICON
  Local_img = bitmap_layer_create(GRect(4, 3, 20, 20));
  bitmap_layer_set_bitmap(Local_img, s_res_takeoff);
  layer_add_child(window_get_root_layer(s_window), (Layer *)Local_img);

//TEMPERATURE
  LocalTemp = text_layer_create(GRect(4, 30, 30, 30)); //(GRect(73, 15, 65, 42));
  text_layer_set_background_color(LocalTemp, GColorClear);
  text_layer_set_text(LocalTemp, strLocalTemp);
  text_layer_set_text_alignment(LocalTemp, GTextAlignmentRight);
  text_layer_set_font(LocalTemp, s_res_roboto_21_condensed);
  layer_add_child(window_get_root_layer(s_window), (Layer *)LocalTemp);
  
    
//////////////////////
// DUAL TIMEZONE #1 //
//////////////////////
	
  //AREA
  DualArea = text_layer_create(GRect(3, 56, 137, 35));
  text_layer_set_font(DualArea, s_res_gothic_14);
  layer_add_child(window_get_root_layer(s_window), (Layer *)DualArea);  
  
	
 //CITY NAME
  s_textlayer_2 = text_layer_create(GRect(28, 55, 100, 17));
  text_layer_set_background_color(s_textlayer_2, GColorClear);
  text_layer_set_text(s_textlayer_2, dualname);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_2);
  
  //DAY OF THE WEEK
  DualDay = text_layer_create(GRect(28, 66, 98, 16));
  text_layer_set_background_color(DualDay, GColorClear);
  text_layer_set_text(DualDay, dualweekday_text);
  text_layer_set_font(DualDay, s_res_gothic_14);
  layer_add_child(window_get_root_layer(s_window), (Layer *)DualDay);
	
  //DATE
  DualDate = text_layer_create(GRect(28, 76, 110, 16));
  text_layer_set_background_color(DualDate, GColorClear);
  text_layer_set_text(DualDate, dualmonth_text);
  text_layer_set_font(DualDate, s_res_gothic_14);
  layer_add_child(window_get_root_layer(s_window), (Layer *)DualDate);
  
 //TIME
  DualTime = text_layer_create(GRect(73, 66, 65, 45));
  text_layer_set_background_color(DualTime, GColorClear);
  text_layer_set_text(DualTime, dualtime_text);
  text_layer_set_text_alignment(DualTime, GTextAlignmentRight);
  text_layer_set_font(DualTime, s_res_roboto_21_condensed);
  layer_add_child(window_get_root_layer(s_window), (Layer *)DualTime);
	
  
//WEATHER//
	
  //ICON
  Dual_img = bitmap_layer_create(GRect(4, 57, 20, 20));
  bitmap_layer_set_bitmap(Dual_img, s_res_landing);
  layer_add_child(window_get_root_layer(s_window), (Layer *)Dual_img);
	
  //TEMPERATURE
  DualTemp = text_layer_create(GRect(4, 73, 20, 20));
  text_layer_set_background_color(DualTemp, GColorClear);
  text_layer_set_text(DualTemp, strDualTemp);
  text_layer_set_text_alignment(DualTemp, GTextAlignmentRight);
  text_layer_set_font(DualTemp, s_res_gothic_14);
  layer_add_child(window_get_root_layer(s_window), (Layer *)DualTemp);
  
//////////////////////
// DUAL TIMEZONE #2 //
//////////////////////
	
  //AREA
  DualArea2 = text_layer_create(GRect(3, 93, 137, 35));
  text_layer_set_font(DualArea2, s_res_gothic_14);
  layer_add_child(window_get_root_layer(s_window), (Layer *)DualArea2);	
	
  //CITY NAME
  DualTZName2 = text_layer_create(GRect(28, 91, 100, 17));
  text_layer_set_background_color(DualTZName2, GColorClear);
  text_layer_set_text(DualTZName2, dualname2);
  layer_add_child(window_get_root_layer(s_window), (Layer *)DualTZName2);
  
  //DAY OF THE WEEK
  DualDay2 = text_layer_create(GRect(28, 102, 98, 16));
  text_layer_set_background_color(DualDay2, GColorClear);
  text_layer_set_text(DualDay2, dualweekday2_text);
  text_layer_set_font(DualDay2, s_res_gothic_14);
  layer_add_child(window_get_root_layer(s_window), (Layer *)DualDay2);
	
  //DATE
  DualDate2 = text_layer_create(GRect(28, 112, 110, 16));
  text_layer_set_background_color(DualDate2, GColorClear);
  text_layer_set_text(DualDate2, dualmonth2_text);
  text_layer_set_font(DualDate2, s_res_gothic_14);
  layer_add_child(window_get_root_layer(s_window), (Layer *)DualDate2);
  	
  //TIME
  DualTime2 = text_layer_create(GRect(73, 102, 65, 45));
  text_layer_set_background_color(DualTime2, GColorClear);
  text_layer_set_text(DualTime2, dualtime2_text);
  text_layer_set_text_alignment(DualTime2, GTextAlignmentRight);
  text_layer_set_font(DualTime2, s_res_roboto_21_condensed);
  layer_add_child(window_get_root_layer(s_window), (Layer *)DualTime2);
	
//WEATHER//
	
  //ICON
  Dual_img2 = bitmap_layer_create(GRect(4, 94, 20, 20));
  bitmap_layer_set_bitmap(Dual_img2, s_res_landing2);
  layer_add_child(window_get_root_layer(s_window), (Layer *)Dual_img2);
	
  //TEMPERATURE
  DualTemp2 = text_layer_create(GRect(4, 110, 20, 20));
  text_layer_set_background_color(DualTemp2, GColorClear);
  text_layer_set_text(DualTemp2, strDualTemp2);
  text_layer_set_text_alignment(DualTemp2, GTextAlignmentRight);
  text_layer_set_font(DualTemp2, s_res_gothic_14);
  layer_add_child(window_get_root_layer(s_window), (Layer *)DualTemp2);
 
 //////////////////////
// DUAL TIMEZONE #3 //
//////////////////////

  //AREA
  DualArea3 = text_layer_create(GRect(3, 130, 137, 35));
  text_layer_set_font(DualArea3, s_res_gothic_14);
  layer_add_child(window_get_root_layer(s_window), (Layer *)DualArea3);
	
  //CITY NAME
  DualTZName3 = text_layer_create(GRect(28, 128, 100, 17));
  text_layer_set_background_color(DualTZName3, GColorClear);
  text_layer_set_text(DualTZName3, dualname3); 
  layer_add_child(window_get_root_layer(s_window), (Layer *)DualTZName3);
	
  //DAY OF THE WEEK
  DualDay3 = text_layer_create(GRect(28, 139, 98, 16));
  text_layer_set_background_color(DualDay3, GColorClear);
  text_layer_set_text(DualDay3, dualweekday3_text);
  text_layer_set_font(DualDay3, s_res_gothic_14);
  layer_add_child(window_get_root_layer(s_window), (Layer *)DualDay3);
	
  //DATE
  DualDate3 = text_layer_create(GRect(28, 149, 110, 16));
  text_layer_set_background_color(DualDate3, GColorClear);
  text_layer_set_text(DualDate3, dualmonth3_text);
  text_layer_set_font(DualDate3, s_res_gothic_14);
  layer_add_child(window_get_root_layer(s_window), (Layer *)DualDate3);
	
  //TIME
  DualTime3 = text_layer_create(GRect(73, 141, 65, 45));
  text_layer_set_background_color(DualTime3, GColorClear);
  text_layer_set_text(DualTime3, dualtime3_text);
  text_layer_set_text_alignment(DualTime3, GTextAlignmentRight);
  text_layer_set_font(DualTime3, s_res_roboto_21_condensed);
  layer_add_child(window_get_root_layer(s_window), (Layer *)DualTime3);
	
//WEATHER//
	
  //ICON
  Dual_img3 = bitmap_layer_create(GRect(4, 131, 20, 20));
  bitmap_layer_set_bitmap(Dual_img3, s_res_landing3);
  layer_add_child(window_get_root_layer(s_window), (Layer *)Dual_img3);
	  
  //DualTemp 2
  DualTemp3 = text_layer_create(GRect(4, 147, 20, 20));
  text_layer_set_background_color(DualTemp3, GColorClear);
  text_layer_set_text(DualTemp3, strDualTemp3);
  text_layer_set_text_alignment(DualTemp3, GTextAlignmentRight);
  text_layer_set_font(DualTemp3, s_res_gothic_14);
  layer_add_child(window_get_root_layer(s_window), (Layer *)DualTemp3);
	
////////////////////
// LAYER INVERTER //
////////////////////
	
	//Invert layer to better differentiate Local and Dual timezones
	inv_layer = inverter_layer_create(GRect(3, 54, 137, 111));
	layer_add_child(window_get_root_layer(s_window), (Layer*) inv_layer);
  
}

static void destroy_ui(void) {
  window_destroy(s_window);
	/*
  text_layer_destroy(LocalArea);
  text_layer_destroy(DualArea);
  text_layer_destroy(LocalTZ);
  text_layer_destroy(LocalTemp);
  text_layer_destroy(LocalTime);
  text_layer_destroy(LocalDate);
  text_layer_destroy(LocalDay);
  text_layer_destroy(DualTime);
  text_layer_destroy(DualDate);
  text_layer_destroy(DualDay);
  text_layer_destroy(DualTemp);
  bitmap_layer_destroy(Local_img);
  bitmap_layer_destroy(Dual_img);
  text_layer_destroy(s_textlayer_1);
  text_layer_destroy(s_textlayer_2);
  gbitmap_destroy(s_res_takeoff);
  gbitmap_destroy(s_res_landing);
  */
}
// END AUTO-GENERATED UI CODE

static void handle_window_unload(Window* window) {
  destroy_ui();
}

void show_main(void) {
  initialise_ui();
  window_set_window_handlers(s_window, (WindowHandlers) {
    .unload = handle_window_unload,
  });
  window_stack_push(s_window, true);
}

void hide_main(void) {
  window_stack_remove(s_window, true);
}

void getDualTime(){
	
	time_t actualPtr = time(NULL);

	
			//Read persistent storage to calculate the time difference
	
			if (persist_exists(LOCAL_TZ_KEY)){localtz = persist_read_int(LOCAL_TZ_KEY);} 
			if (persist_exists(DUAL_TZ_KEY)){dualtz = persist_read_int(DUAL_TZ_KEY);}
			if (persist_exists(DUAL_TZ2_KEY)){dualtz2 = persist_read_int(DUAL_TZ2_KEY);}
			if (persist_exists(DUAL_TZ3_KEY)){dualtz3 = persist_read_int(DUAL_TZ3_KEY);}
			
		
			timediff= dualtz - localtz;
			timediff2= dualtz2 - localtz;
			timediff3= dualtz3 - localtz;
	
			
			//Define and Calculate Time Zones
			//TIME ZONE 1
			struct tm *tzPtr = gmtime(&actualPtr);
		
			tzPtr->tm_sec += timediff;
			//Since mktime() is not realible in Pebble's firmware, use PUtils to built the dual time.
			time_t dualtime = p_mktime(tzPtr);
			struct tm *tz1Ptr = gmtime(&dualtime);
	
	
			if (clock_is_24h_style()){strftime(dualtime_text, sizeof(dualtime_text), "%H:%M", tz1Ptr);}
			else {strftime(dualtime_text, sizeof(dualtime_text), "%I:%M", tz1Ptr);}
	

				//Remove the leading 0s
				if (dualtime_text[0]=='0') {memcpy(&dualtime_text," ",1);}
	
			//Calculate the Dual Zone Date
			char *sys_locale = setlocale(LC_ALL, "");

			if (strcmp("en_US", sys_locale) == 0) {
			  strftime(dualmonth_text,sizeof(dualmonth_text),"%b %e %Y",tz1Ptr);
			
			} else {
			  strftime(dualmonth_text,sizeof(dualmonth_text),"%e %b %Y",tz1Ptr);
			}
			
			strftime(dualweekday_text,sizeof(dualweekday_text),"%A",tz1Ptr);
	
			//TIME ZONE 2
			struct tm *tzPtr2 = gmtime(&actualPtr);
		
			tzPtr2->tm_sec += timediff2;
			//Since mktime() is not realible in Pebble's firmware, use PUtils to built the dual time.
			time_t dualtime2 = p_mktime(tzPtr2);
			struct tm *tz2Ptr = gmtime(&dualtime2);
	
	
			if (clock_is_24h_style()){strftime(dualtime2_text, sizeof(dualtime2_text), "%H:%M", tz2Ptr);}
			else {strftime(dualtime2_text, sizeof(dualtime2_text), "%I:%M", tz2Ptr);}
	

				//Remove the leading 0s
				if (dualtime2_text[0]=='0') {memcpy(&dualtime2_text," ",1);}
	
			//Calculate the Dual Zone Date
			//char *sys_locale = setlocale(LC_ALL, "");

			if (strcmp("en_US", sys_locale) == 0) {
			  strftime(dualmonth2_text,sizeof(dualmonth2_text),"%b %e %Y",tz2Ptr);
			
			} else {
			  strftime(dualmonth2_text,sizeof(dualmonth2_text),"%e %b %Y",tz2Ptr);
			}
			
			strftime(dualweekday2_text,sizeof(dualweekday2_text),"%A",tz2Ptr);
	
			//TIME ZONE 3
			struct tm *tzPtr3 = gmtime(&actualPtr);
		
			tzPtr3->tm_sec += timediff3;
			//Since mktime() is not realible in Pebble's firmware, use PUtils to built the dual time.
			time_t dualtime3 = p_mktime(tzPtr3);
			struct tm *tz3Ptr = gmtime(&dualtime3);
	
	
			if (clock_is_24h_style()){strftime(dualtime3_text, sizeof(dualtime3_text), "%H:%M", tz3Ptr);}
			else {strftime(dualtime3_text, sizeof(dualtime3_text), "%I:%M", tz3Ptr);}
	

				//Remove the leading 0s
				if (dualtime3_text[0]=='0') {memcpy(&dualtime3_text," ",1);}
	
			//Calculate the Dual Zone Date
			//char *sys_locale = setlocale(LC_ALL, "");

			if (strcmp("en_US", sys_locale) == 0) {
			  strftime(dualmonth3_text,sizeof(dualmonth3_text),"%b %e %Y",tz3Ptr);
			
			} else {
			  strftime(dualmonth3_text,sizeof(dualmonth3_text),"%e %b %Y",tz3Ptr);
			}
			
			strftime(dualweekday3_text,sizeof(dualweekday3_text),"%A",tz3Ptr);
			
	
			//DISPLAY THE TIME ZONES	
			text_layer_set_text(DualTime, dualtime_text);
			text_layer_set_text(DualDay,dualweekday_text);
			text_layer_set_text(DualDate,dualmonth_text); 
			//if(!clock_is_24h_style()){strftime(strDualAMPMInd, sizeof(strDualAMPMInd), "%p", tz1Ptr);} 
			text_layer_set_text(DualTime2, dualtime2_text);
			text_layer_set_text(DualDay2,dualweekday2_text);
			text_layer_set_text(DualDate2,dualmonth2_text);
	
			text_layer_set_text(DualTime3, dualtime3_text);
			text_layer_set_text(DualDay3,dualweekday3_text);
			text_layer_set_text(DualDate3,dualmonth3_text);
}

//*********************************//
//** Get the current date & time **//
//*********************************//
void getDate()
{
	//Get the date
	time_t actualPtr = time(NULL);
	struct tm *tz1Ptr = gmtime(&actualPtr);
	
	//get the local date
	char *sys_locale = setlocale(LC_ALL, "");
		
	if (strcmp("en_US", sys_locale) == 0) {
		strftime(localmonth_text,sizeof(localmonth_text),"%b %e %Y",tz1Ptr);
			
	} else {
		strftime(localmonth_text,sizeof(localmonth_text),"%e %b %Y",tz1Ptr);
	}
	
	strftime(localweekday_text,sizeof(localweekday_text),"%A",tz1Ptr);
	
	//concatenate week day and date
	//clean up the string
	memset(&localdate_text[0], 0, sizeof(localdate_text));
	strcat(localdate_text, localweekday_text);
	strcat(localdate_text, ", ");
	strcat(localdate_text, localmonth_text);
	//set the layers
	//text_layer_set_text(LocalDay,localweekday_text);
	//text_layer_set_text(LocalDate,localmonth_text); 
	text_layer_set_text(LocalDate,localdate_text); 
}

void getTime()
	{
		clock_copy_time_string(localtime_text, sizeof(localtime_text));
			
		//Set the Local Time
        text_layer_set_text(LocalTime, localtime_text);
		
		//Set the Dual Time
		getDualTime();
	
}

//************************//
// Capture the Tick event //
//************************//
void handle_tick(struct tm *tick_time, TimeUnits units_changed)
{

			//Set the AM/PM indicator
			//if(clock_is_24h_style()){memcpy(&ampm_text,  "24H", strlen("24H"));}
			//else {strftime(ampm_text, sizeof(ampm_text), "%p", tick_time);}
			//text_layer_set_text(ampm_layer, ampm_text); //Update the weekday layer  


       if (units_changed & MINUTE_UNIT)
       {

		   //update the time
		   //if(!clock_is_24h_style()){strftime(strLocalAMPMInd, sizeof(strLocalAMPMInd), "%p", tick_time);} 
		   
		   if (clock_is_24h_style()){strftime(localtime_text, sizeof(localtime_text), "%H:%M", tick_time);}
				else {strftime(localtime_text, sizeof(localtime_text), "%I:%M", tick_time);
					 strftime(strLocalAMPMInd, sizeof(strLocalAMPMInd), "%p", tick_time);}
	

				//Remove the leading 0s
				if (localtime_text[0]=='0') {memcpy(&localtime_text," ",1);}
		   
			//getTime();
		//Set the Dual Time
		getDualTime();


       } //MINUTE CHANGES
	   if (units_changed & DAY_UNIT){
			//Update the date
			getDate();}
	
} //HANDLE_TICK

//*****************//
// AppSync options //
//*****************//

/* simple base 10 only itoa */
char *
itoa10 (int value, char *result)
{
    char const digit[] = "0123456789";
    char *p = result;
    if (value < 0) {
        *p++ = '-';
        value *= -1;
    }

    /* move number of required chars and null terminate */
    int shift = value;
    do {
        ++p;
        shift /= 10;
    } while (shift);
    *p = '\0';

    /* populate result in reverse order */
    do {
        *--p = digit [value % 10];
        value /= 10;
    } while (value);

    return result;
}

  static AppSync sync;
  static uint8_t sync_buffer[512];

  static void sync_tuple_changed_callback(const uint32_t key,
                                        const Tuple* new_tuple,
                                        const Tuple* old_tuple,
                                        void* context) {

        
  // App Sync keeps new_tuple in sync_buffer, so we may use it directly
  switch (key) {
	  case LOCAL_NAME_KEY:
	  		persist_write_string(LOCAL_NAME_KEY, new_tuple->value->cstring);
	  		//APP_LOG(APP_LOG_LEVEL_DEBUG, new_tuple->value->cstring);
      		break;
	  case LOCAL_TZ_KEY:
	      	//persist_write_int(LOCAL_TZ_KEY, new_tuple->value->uint16);
	 		persist_write_int(LOCAL_TZ_KEY, new_tuple->value->uint32);
	  
	  		//debug
	  		//static char strdebug[15];
	  		//itoa10(new_tuple->value->uint32, strdebug);
	  		//APP_LOG(APP_LOG_LEVEL_DEBUG, strdebug);
	  		//debug - END
	  
      		break;
	  case LOCAL_TZNAME_KEY:
	  		persist_write_string(LOCAL_TZNAME_KEY, new_tuple->value->cstring);
	  		//APP_LOG(APP_LOG_LEVEL_DEBUG, new_tuple->value->cstring);
      		break;
	  case DUAL_NAME_KEY:
	  		persist_write_string(DUAL_NAME_KEY, new_tuple->value->cstring);
	  		//APP_LOG(APP_LOG_LEVEL_DEBUG, new_tuple->value->cstring);
      		break;
	  case DUAL_TZ_KEY:
	      	persist_write_int(DUAL_TZ_KEY, new_tuple->value->uint32);
		  	//debug
	  		//static char strdebug2[15];
	  		//itoa10(new_tuple->value->uint32, strdebug2);
	  		//APP_LOG(APP_LOG_LEVEL_DEBUG, strdebug2);
	  		//debug - END
      		break;
	  case DUAL_TZNAME_KEY:
	  		persist_write_string(DUAL_TZNAME_KEY, new_tuple->value->cstring);
	  		//APP_LOG(APP_LOG_LEVEL_DEBUG, new_tuple->value->cstring);
      		break;
	  
	  case LOCAL_TEMP_KEY:
	  		persist_write_string(LOCAL_TEMP_KEY, new_tuple->value->cstring);
	  		//APP_LOG(APP_LOG_LEVEL_DEBUG, new_tuple->value->cstring);
      		break;
	  case LOCAL_ICON_KEY:
	  		persist_write_int(LOCAL_ICON_KEY, new_tuple->value->uint8);
	  		//APP_LOG(APP_LOG_LEVEL_DEBUG, new_tuple->value->cstring); 
      		break;
	  
	  
	  case DUAL_TEMP_KEY:
	  		persist_write_string(DUAL_TEMP_KEY, new_tuple->value->cstring);
	  		//APP_LOG(APP_LOG_LEVEL_DEBUG, new_tuple->value->cstring);
      		break;
	  
	  case DUAL_ICON_KEY:
	  		persist_write_int(DUAL_ICON_KEY, new_tuple->value->uint8);
	  		//APP_LOG(APP_LOG_LEVEL_DEBUG, new_tuple->value->cstring); 
      		break;
	  
	  //Dual Timezone 2
	  case DUAL_NAME2_KEY:
	  		persist_write_string(DUAL_NAME2_KEY, new_tuple->value->cstring);
	  		//APP_LOG(APP_LOG_LEVEL_DEBUG, new_tuple->value->cstring);
      		break;
	  
	  case DUAL_TZ2_KEY:
	      	persist_write_int(DUAL_TZ2_KEY, new_tuple->value->uint32);
		  	break;
	  
	  case DUAL_TEMP2_KEY:
	  		persist_write_string(DUAL_TEMP2_KEY, new_tuple->value->cstring);
	  		//APP_LOG(APP_LOG_LEVEL_DEBUG, new_tuple->value->cstring);
      		break;
	  
	  case DUAL_ICON2_KEY:
	  		persist_write_int(DUAL_ICON2_KEY, new_tuple->value->uint8);
	  		//APP_LOG(APP_LOG_LEVEL_DEBUG, new_tuple->value->cstring); 
      		break;
	  
	  //Dual Timezone 3
	  case DUAL_NAME3_KEY:
	  		persist_write_string(DUAL_NAME3_KEY, new_tuple->value->cstring);
	  		//APP_LOG(APP_LOG_LEVEL_DEBUG, new_tuple->value->cstring);
      		break;
	  
	  case DUAL_TZ3_KEY:
	      	persist_write_int(DUAL_TZ3_KEY, new_tuple->value->uint32);
		  	break;
	  
	  case DUAL_TEMP3_KEY:
	  		persist_write_string(DUAL_TEMP3_KEY, new_tuple->value->cstring);
	  		//APP_LOG(APP_LOG_LEVEL_DEBUG, new_tuple->value->cstring);
      		break;
	  
	  case DUAL_ICON3_KEY:
	  		persist_write_int(DUAL_ICON3_KEY, new_tuple->value->uint8);
	  		//APP_LOG(APP_LOG_LEVEL_DEBUG, new_tuple->value->cstring); 
      		break;

  }
	  
	  //refresh the screen
	  
		if (persist_exists(LOCAL_NAME_KEY)){persist_read_string(LOCAL_NAME_KEY, localname, sizeof(localname));}
	  	if (persist_exists(DUAL_NAME_KEY)){persist_read_string(DUAL_NAME_KEY, dualname, sizeof(dualname));}
	  	if (persist_exists(LOCAL_TZNAME_KEY)){persist_read_string(LOCAL_TZNAME_KEY, LocalTZName, sizeof(LocalTZName));}
	  	if (persist_exists(DUAL_TZNAME_KEY)){persist_read_string(DUAL_TZNAME_KEY, DualTZName, sizeof(DualTZName));}
	  	if (persist_exists(LOCAL_TEMP_KEY)){persist_read_string(LOCAL_TEMP_KEY, strLocalTemp, sizeof(strLocalTemp));}
	  	if (persist_exists(DUAL_TEMP_KEY)){persist_read_string(DUAL_TEMP_KEY, strDualTemp, sizeof(strDualTemp));}
	  	if (persist_exists(LOCAL_ICON_KEY)){
			if (s_res_takeoff != NULL){gbitmap_destroy(s_res_takeoff);}
			s_res_takeoff = gbitmap_create_with_resource(WEATHER_ICONS[persist_read_int(LOCAL_ICON_KEY)]);
			bitmap_layer_set_bitmap(Local_img, s_res_takeoff);
		}
	  	if (persist_exists(DUAL_ICON_KEY)){
			if (s_res_landing != NULL){gbitmap_destroy(s_res_landing);}
			s_res_landing = gbitmap_create_with_resource(WEATHER_ICONS[persist_read_int(DUAL_ICON_KEY)]);
			bitmap_layer_set_bitmap(Dual_img, s_res_landing);
		}
	 
		//Dual Timezone 2
	  if (persist_exists(DUAL_NAME2_KEY)){persist_read_string(DUAL_NAME2_KEY, dualname2, sizeof(dualname2));}
	  if (persist_exists(DUAL_TEMP2_KEY)){persist_read_string(DUAL_TEMP2_KEY, strDualTemp2, sizeof(strDualTemp2));}
	  if (persist_exists(DUAL_ICON2_KEY)){
			if (s_res_landing2 != NULL){gbitmap_destroy(s_res_landing2);}
			s_res_landing2 = gbitmap_create_with_resource(WEATHER_ICONS[persist_read_int(DUAL_ICON2_KEY)]);
			bitmap_layer_set_bitmap(Dual_img2, s_res_landing2);
		}
	  
	  //Dual Timezone 3
	  if (persist_exists(DUAL_NAME3_KEY)){persist_read_string(DUAL_NAME3_KEY, dualname3, sizeof(dualname3));}
	  if (persist_exists(DUAL_TEMP3_KEY)){persist_read_string(DUAL_TEMP3_KEY, strDualTemp3, sizeof(strDualTemp3));}
	  if (persist_exists(DUAL_ICON3_KEY)){
			if (s_res_landing3 != NULL){gbitmap_destroy(s_res_landing3);}
			s_res_landing3 = gbitmap_create_with_resource(WEATHER_ICONS[persist_read_int(DUAL_ICON3_KEY)]);
			bitmap_layer_set_bitmap(Dual_img3, s_res_landing3);
		}	 
	  	
	  
}

//************************************************//
// TIMER to refresh the weather data every 30 min //
//************************************************//
static void send_cmd(void) {

         Tuplet value = MyTupletCString(2, LocalTZName);
        
         DictionaryIterator *iter;
         app_message_outbox_begin(&iter);
        
         if (iter == NULL) {
                return;
         }
        
         dict_write_tuplet(iter, &value);
         dict_write_end(iter);
        
         app_message_outbox_send();
	
}

static void timer_callback(void *context) {

		//Developer vibe: confirm that timer is not killed
		//vibes_double_pulse();
	
        timer = app_timer_register(timeout_ms, timer_callback, NULL);

        //Refresh the weather
        send_cmd();
	       

}



//*************//
// ENTRY POINT //
//*************//
void SetupMessages(){

	
                app_message_open(inbound_size, outbound_size);
        
                Tuplet initial_values[] = {
					MyTupletCString(LOCAL_NAME_KEY, localname),
					TupletInteger(LOCAL_TZ_KEY, persist_read_int(LOCAL_TZ_KEY)), 
					MyTupletCString(LOCAL_TZNAME_KEY, LocalTZName),
					MyTupletCString(DUAL_NAME_KEY, dualname),
					TupletInteger(DUAL_TZ_KEY, persist_read_int(DUAL_TZ_KEY)),
					MyTupletCString(DUAL_TZNAME_KEY, DualTZName),
					MyTupletCString(LOCAL_TEMP_KEY, strLocalTemp),
					MyTupletCString(DUAL_TEMP_KEY, strDualTemp),
					TupletInteger(LOCAL_ICON_KEY, persist_read_int(LOCAL_ICON_KEY)), 
					TupletInteger(DUAL_ICON_KEY, persist_read_int(DUAL_ICON_KEY)), 
					//Dual Timezone 2
					MyTupletCString(DUAL_NAME2_KEY, dualname2),
					TupletInteger(DUAL_TZ2_KEY, 0),
					MyTupletCString(DUAL_TEMP2_KEY, ""),
					TupletInteger(DUAL_ICON2_KEY, 0), 
					//Dual Timezone 3
					MyTupletCString(DUAL_NAME3_KEY, dualname3),
					TupletInteger(DUAL_TZ3_KEY, 0),
					MyTupletCString(DUAL_TEMP3_KEY, ""),
					TupletInteger(DUAL_ICON3_KEY, 0), 
					
                }; //TUPLET INITIAL VALUES
        
                app_sync_init(&sync, sync_buffer, sizeof(sync_buffer), initial_values,
                ARRAY_LENGTH(initial_values), sync_tuple_changed_callback,
                NULL, NULL);
}


void handle_init(void)
{
	
	//Use the internationalization API to detect the user's language
	setlocale(LC_ALL, i18n_get_system_locale());
	
	//Read the persistent storage
	if (persist_exists(LOCAL_NAME_KEY)){persist_read_string(LOCAL_NAME_KEY, localname, sizeof(localname));}
	if (persist_exists(DUAL_NAME_KEY)){persist_read_string(DUAL_NAME_KEY, dualname, sizeof(dualname));}
	if (persist_exists(LOCAL_TZNAME_KEY)){persist_read_string(LOCAL_TZNAME_KEY, LocalTZName, sizeof(LocalTZName));}
	if (persist_exists(DUAL_TZNAME_KEY)){persist_read_string(DUAL_TZNAME_KEY, DualTZName, sizeof(DualTZName));}
	if (persist_exists(LOCAL_TEMP_KEY)){persist_read_string(LOCAL_TEMP_KEY, strLocalTemp, sizeof(strLocalTemp));}
	if (persist_exists(DUAL_TEMP_KEY)){persist_read_string(DUAL_TEMP_KEY, strDualTemp, sizeof(strDualTemp));}
	
	if (persist_exists(LOCAL_ICON_KEY)){s_res_takeoff = gbitmap_create_with_resource(WEATHER_ICONS[persist_read_int(LOCAL_ICON_KEY)]);}
	else{s_res_takeoff = gbitmap_create_with_resource(RESOURCE_ID_TakeOff);}
	
	if (persist_exists(DUAL_ICON_KEY)){s_res_landing = gbitmap_create_with_resource(WEATHER_ICONS[persist_read_int(DUAL_ICON_KEY)]);}
	else{s_res_landing = gbitmap_create_with_resource(RESOURCE_ID_Landing);}
	
	//Dual Timezone 2
	if (persist_exists(DUAL_NAME2_KEY)){persist_read_string(DUAL_NAME2_KEY, dualname2, sizeof(dualname2));}
	if (persist_exists(DUAL_TEMP2_KEY)){persist_read_string(DUAL_TEMP2_KEY, strDualTemp2, sizeof(strDualTemp2));}
	if (persist_exists(DUAL_ICON2_KEY)){s_res_landing2 = gbitmap_create_with_resource(WEATHER_ICONS[persist_read_int(DUAL_ICON2_KEY)]);}
	else{s_res_landing2 = gbitmap_create_with_resource(RESOURCE_ID_Landing);}
	
	//Dual Timezone 3
	if (persist_exists(DUAL_NAME3_KEY)){persist_read_string(DUAL_NAME3_KEY, dualname3, sizeof(dualname3));}
	if (persist_exists(DUAL_TEMP3_KEY)){persist_read_string(DUAL_TEMP3_KEY, strDualTemp3, sizeof(strDualTemp3));}
	if (persist_exists(DUAL_ICON3_KEY)){s_res_landing3 = gbitmap_create_with_resource(WEATHER_ICONS[persist_read_int(DUAL_ICON3_KEY)]);}
	else{s_res_landing3 = gbitmap_create_with_resource(RESOURCE_ID_Landing);}
	
	//Display the UI
	show_main();
	
	//Init the AppSync with the js code
	SetupMessages();
	
	//Setup the date & time
	getDate();
	//getTime();
	
	//subscribe to the tick event
	time_t now = time(NULL);
	struct tm *current_time = localtime(&now);

	handle_tick(current_time, MINUTE_UNIT);
	tick_timer_service_subscribe(MINUTE_UNIT, &handle_tick);
	
	//setup the timer to refresh the weather info every 30min
	timer = app_timer_register(timeout_ms, timer_callback, NULL);
}

void handle_deinit(void)
{
	hide_main();
}


int main(void)
{        
        handle_init();
        app_event_loop();
        handle_deinit();
}

