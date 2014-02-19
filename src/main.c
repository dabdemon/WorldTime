//Declare and Import references
#include "pebble.h"
#include "pebble_fonts.h"


/*
//Watchface header section
#define MY_UUID { 0xE9, 0x96, 0x5A, 0xC7, 0x5E, 0xA1, 0x49, 0xC1, 0xA6, 0xC3, 0x05, 0xAD, 0xA8, 0x4F, 0xBF, 0x2A }


PBL_APP_INFO(MY_UUID,
             "World Time", "dabdemon", 
             1, 0, // App version
             RESOURCE_ID_IMAGE_MENU_ICON,
             APP_INFO_WATCH_FACE);
*/		 

#define WEEKDAY_FRAME	  (GRect(5,  2, 95, 168-145)) 
#define BATT_FRAME 	      (GRect(100,  4, 40, 168-146)) 
#define BT_FRAME 	      (GRect(125,  4, 25, 168-146)) 
#define TIME_FRAME        (GRect(0, 15, 144, 168-16)) 
#define DATE_FRAME        (GRect(1, 69, 139, 168-62)) 

#define WC1NAME_FRAME     (GRect(5,  95, 105, 168-145))
#define WC1TIME_FRAME     (GRect(100, 95, 41, 168-145))
	
#define WC2NAME_FRAME     (GRect(5,  110, 105, 168-145))
#define WC2TIME_FRAME     (GRect(100, 110, 41, 168-145))
	
#define WC3NAME_FRAME     (GRect(5,  125, 105, 168-145))
#define WC3TIME_FRAME     (GRect(100, 125, 41, 168-145))
	
#define WC4NAME_FRAME     (GRect(5,  140, 105, 168-145))
#define WC4TIME_FRAME     (GRect(100, 140, 41, 168-145))

//******************//
// DEFINE THE ICONS //
//******************//	
static int LAYER_ICONS[] = {
	RESOURCE_ID_BT_CONNECTED,
	RESOURCE_ID_BT_DISCONNECTED,
};


//Declare initial window	
	Window *my_window;    

//Define the layers
	TextLayer *date_layer;   		// Layer for the date
	TextLayer *Time_Layer; 			// Layer for the time
	TextLayer *Weekday_Layer; 		//Layer for the weekday
	TextLayer *Last_Update; 		// Layer for the last update
	TextLayer *Location_Layer; 		// Layer for the last update
	TextLayer *Batt_Layer;			//Layer for the BT connection
	TextLayer *BT_Layer;			//Layer for the BT connection
	TextLayer *WC1NAME_Layer;	//Layer for the Temperature
	TextLayer *WC1TIME_Layer;	//Layer for the Temperature
	TextLayer *WC2NAME_Layer;	//Layer for the Temperature
	TextLayer *WC2TIME_Layer;	//Layer for the Temperature
	TextLayer *WC3NAME_Layer;	//Layer for the Temperature
	TextLayer *WC3TIME_Layer;	//Layer for the Temperature
	TextLayer *WC4NAME_Layer;	//Layer for the Temperature
	TextLayer *WC4TIME_Layer;	//Layer for the Temperature

/*
	TextLayer *Max_Layer;			//Layer for the Max Temperature
	TextLayer *Min_Layer;			//Layer for the Min Temperature
	*/
	static GBitmap *BT_image;
	static BitmapLayer *BT_icon_layer; //Layer for the BT connection

	static GBitmap *Batt_image;
	static BitmapLayer *Batt_icon_layer; //Layer for the Battery status


//Define and initialize variables
	//FONTS
	GFont font_date;        // Font for date
	GFont font_time;        // Font for time
	GFont font_update;      // Font for last update
	GFont font_temperature;	// Font for the temperature

	//Vibe Control
	bool BTConnected = true;

	//Date & Time	
	static char last_update[]="00:00 ";
	static int initial_minute;

	static char weekday_text[] = "XXXXXXXXXX";
	static char date_text[] = "XXX 00";
	static char month_text[] = "XXXXXXXXXXXXX";
	static char day_text[] = "31";
	static char day_month[]= "31 SEPTEMBER"; 
	static char time_text[] = "00:00"; 

	//WORLD CLOCK
	static char tz1_name[]="XXXXXXXXXXXXX";
	int tz1_hours= 0;
	int tz1_min=0;
	static char tz2_name[]="XXXXXXXXXXXXX";
	int tz2_hours= 0;
	int tz2_min=0;
	static char tz3_name[]="XXXXXXXXXXXXX";
	int tz3_hours= 0;
	int tz3_min=0;
	static char tz4_name[]="XXXXXXXXXXXXX";
	int tz4_hours= 0;
	int tz4_min=0;
	
	static char TZ1[] = "00:00";
	static char TZ2[] = "00:00";
	static char TZ3[] = "00:00";
	static char TZ4[] = "00:00";

	bool translate_sp = true;

enum TimeZoneKey {
  TZ1Name_KEY = 0x0,        // TUPLE_CSTRING 
  TZ1Hour_KEY = 0x1, 		// TUPLE_INT
  TZ1Minute_KEY = 0x2,      // TUPLE_INT
  TZ2Name_KEY = 0x3,        // TUPLE_CSTRING 
  TZ2Hour_KEY = 0x4, 		// TUPLE_INT
  TZ2Minute_KEY = 0x5,      // TUPLE_INT
  TZ3Name_KEY = 0x6,        // TUPLE_CSTRING 
  TZ3Hour_KEY = 0x7, 		// TUPLE_INT
  TZ3Minute_KEY = 0x8,      // TUPLE_INT
  TZ4Name_KEY = 0x9,        // TUPLE_CSTRING 
  TZ4Hour_KEY = 0x10, 		// TUPLE_INT
  TZ4Minute_KEY = 0x11,     // TUPLE_INT
  INVERT_COLOR_KEY = 0x12,  // TUPLE_INT
};

//**************************//
// Check the Battery Status //
//**************************//

static void handle_battery(BatteryChargeState charge_state) {
  	static char battery_text[] = "100%";

  if (charge_state.is_charging) {
    //snprintf(battery_text, sizeof(battery_text), "charging");
	  Batt_image = gbitmap_create_with_resource(RESOURCE_ID_BATT_CHAR);
	  bitmap_layer_set_bitmap(Batt_icon_layer, Batt_image);
  } else {
    snprintf(battery_text, sizeof(battery_text), "%d%%", charge_state.charge_percent);
	  //if ((battery_text[0] == "1" || battery_text[0] == "2")  && strlen(battery_text)<4) //If the charge is between 0% and 20%
	  if (charge_state.charge_percent<20) //If the charge is between 0% and 20%
	  {
		Batt_image = gbitmap_create_with_resource(RESOURCE_ID_BATT_EMPTY);
	  	bitmap_layer_set_bitmap(Batt_icon_layer, Batt_image);
  }
  //text_layer_set_text(Batt_Layer, battery_text);
}
}

//******************************//
// Handle Bluetooth Connection  //
//*****************************//
static void handle_bluetooth(bool connected) 
{
  	//text_layer_set_text(BT_Layer, connected ? "C" : "D");

	//draw the BT icon if connected

	if(connected ==true)
	{
		BT_image = gbitmap_create_with_resource(RESOURCE_ID_BT_CONNECTED);
  		bitmap_layer_set_bitmap(BT_icon_layer, BT_image);
		if (BTConnected == false){
			//Vibes to alert connection
			vibes_double_pulse();
			BTConnected = true;
		}
	}
	else
	{
		BT_image = gbitmap_create_with_resource(RESOURCE_ID_BT_DISCONNECTED);
  		bitmap_layer_set_bitmap(BT_icon_layer, BT_image);
		if (BTConnected == true){
			//Vibes to alert disconnection
			vibes_long_pulse();
			BTConnected = false;
		}

	}


} //handle_bluetooth



//*****************//
// AppSync options //
//*****************//

        static AppSync sync;
        static uint8_t sync_buffer[256];



        static void sync_tuple_changed_callback(const uint32_t key,
                                        const Tuple* new_tuple,
                                        const Tuple* old_tuple,
                                        void* context) {

        
  // App Sync keeps new_tuple in sync_buffer, so we may use it directly
  switch (key) {
    case TZ1Name_KEY:
  		persist_write_string(TZ1Name_KEY, new_tuple->value->cstring);
	  	memcpy(&tz1_name, new_tuple->value->cstring, strlen(new_tuple->value->cstring));
	  	text_layer_set_text(WC1NAME_Layer, new_tuple->value->cstring);
      	break;

    case TZ1Hour_KEY:
		tz1_hours = new_tuple->value->int8;	  
	  	persist_write_int(TZ1Hour_KEY, tz1_hours);
     	break;

     case TZ1Minute_KEY:
		tz1_min = new_tuple->value->int8;  
	  	persist_write_int(TZ1Minute_KEY, tz1_min);
      	break;
	  
	 case TZ2Name_KEY:
  		persist_write_string(TZ2Name_KEY, new_tuple->value->cstring);
	  	memcpy(&tz2_name, new_tuple->value->cstring, strlen(new_tuple->value->cstring));
	  	text_layer_set_text(WC2NAME_Layer, new_tuple->value->cstring);
      	break;

    case TZ2Hour_KEY:
	  	persist_write_int(TZ2Hour_KEY, new_tuple->value->int8);
		tz2_hours = new_tuple->value->int8;
      	break;

     case TZ2Minute_KEY:
	  	persist_write_int(TZ2Minute_KEY, new_tuple->value->int8);
		tz2_min = new_tuple->value->int8;
      	break;
	  
	 case TZ3Name_KEY:
  		persist_write_string(TZ3Name_KEY, new_tuple->value->cstring);
	  	memcpy(&tz3_name, new_tuple->value->cstring, strlen(new_tuple->value->cstring));
	  	text_layer_set_text(WC3NAME_Layer, new_tuple->value->cstring);
      	break;

    case TZ3Hour_KEY:
	  	persist_write_int(TZ3Hour_KEY, new_tuple->value->int8);
		tz3_hours = new_tuple->value->int8;
      	break;

     case TZ3Minute_KEY:
	  	persist_write_int(TZ3Minute_KEY, new_tuple->value->int8);
		tz3_min = new_tuple->value->int8;
      	break;

	 case TZ4Name_KEY:
  		persist_write_string(TZ4Name_KEY, new_tuple->value->cstring);
	  	memcpy(&tz4_name, new_tuple->value->cstring, strlen(new_tuple->value->cstring));
	  	text_layer_set_text(WC4NAME_Layer, new_tuple->value->cstring);
      	break;

    case TZ4Hour_KEY:
	  	persist_write_int(TZ4Hour_KEY, new_tuple->value->int8);
		tz4_hours = new_tuple->value->int8;
      	break;

     case TZ4Minute_KEY:
	  	persist_write_int(TZ4Minute_KEY, new_tuple->value->int8);
		tz4_min = new_tuple->value->int8;
      	break;


	 case INVERT_COLOR_KEY:
		  //color_inverted = new_tuple->value->uint8 != 0;
		  persist_write_bool(INVERT_COLOR_KEY, new_tuple->value->uint8 != 0);

	  	  //refresh the layout
	  	  //InvertColors(color_inverted);
		  break;
  }
}


void TranslateDate(){

			if (month_text[0] == 'J' && month_text[1] == 'a')
			{
				memcpy(&month_text, "   enero", strlen("   enero")+1); // January
			}

			if (month_text[0] == 'F' && month_text[1] == 'e')
			{
				memcpy(&month_text, "   febrero", strlen("   febrero")+1); // Febrary
			}

			if (month_text[0] == 'M' && month_text[2] == 'r')
			{
				memcpy(&month_text, "   marzo", strlen("   marzo")+1); // March
			}

			if (month_text[0] == 'A' && month_text[1] == 'p')
			{
				memcpy(&month_text, "   abril", strlen("   abril")+1); // April
			}

			if (month_text[0] == 'M' && month_text[2] == 'y')
			{
				memcpy(&month_text, "   de mayo", strlen("   de mayo")+1); // May
			}

			if (month_text[0] == 'J' && month_text[2] == 'n')
			{
				memcpy(&month_text, "   junio", strlen("   junio")+1); // June
			}

			if (month_text[0] == 'J' && month_text[2] == 'l')
			{
				memcpy(&month_text, "   julio", strlen("   julio")+1); // July
			}

			if (month_text[0] == 'A' && month_text[1] == 'u')
			{
				memcpy(&month_text, "   agosto ", strlen("   agosto ")+1); // August
			}

			if (month_text[0] == 'S' && month_text[1] == 'e')
			{
				memcpy(&month_text, "   septiembre", strlen("   septiembre")+1); // September
			}

			if (month_text[0] == 'O' && month_text[1] == 'c')
			{
				memcpy(&month_text, "   octubre", strlen("   octubre")+1); // October
			}

			if (month_text[0] == 'N' && month_text[1] == 'o')
			{
				memcpy(&month_text, "   noviembre", strlen("   noviembre")+1); // November
			}

			if (month_text[0] == 'D' && month_text[1] == 'e')
			{
				memcpy(&month_text, "   diciembre", strlen("   diciembre")+1); // December
			}

			// Primitive hack to translate the day of week to another language
			// Needs to be exactly 3 characters, e.g. "Mon" or "Mo "
			// Supported characters: A-Z, a-z, 0-9

			if (weekday_text[0] == 'M')
			{
				memcpy(&weekday_text, " Lunes", strlen(" Lunes")+1); // Monday
			}

			if (weekday_text[0] == 'T' && weekday_text[1] == 'u')
			{
				memcpy(&weekday_text, " Martes", strlen(" Martes")+1); // Tuesday
			}

			if (weekday_text[0] == 'W')
			{
				memcpy(&weekday_text, " Miercoles", strlen(" Miercoles")+1); // Wednesday
			}

			if (weekday_text[0] == 'T' && weekday_text[1] == 'h')
			{
				memcpy(&weekday_text, " Jueves", strlen(" Jueves")+1); // Thursday
			}

			if (weekday_text[0] == 'F')
			{
				memcpy(&weekday_text, " Viernes", strlen(" Viernes")+1); // Friday
			}

			if (weekday_text[0] == 'S' && weekday_text[1] == 'a')
			{
				memcpy(&weekday_text, " Sabado", strlen(" Sabado")+1); // Saturday
			}

			if (weekday_text[0] == 'S' && weekday_text[1] == 'u')
			{
				memcpy(&weekday_text, " Domingo", strlen(" Domingo")+1); // Sunday
			}


}



//************************//
// Capture the Tick event //
//************************//
void handle_tick(struct tm *tick_time, TimeUnits units_changed)
{

	//time_t lnow = time(NULL);
	struct tm local_time;

//Init the date

				//Get the Weekday
				strftime(weekday_text,sizeof(weekday_text),"%A",tick_time);
				//Get the Month + Day (English format)
				 strftime(month_text,sizeof(month_text),"%B %e",tick_time);
				//Get the Day + Month (Spanish format)
				strftime(day_month,sizeof(day_month),"%e %B",tick_time);


				if(translate_sp){
					//Get the Month
					strftime(month_text,sizeof(month_text),"%B",tick_time);
					//Get the day
					strftime(day_text,sizeof(day_text),"%e",tick_time);
					//Translate to Spanish
					TranslateDate();

					//Concatenate the day to the month
					memcpy(&month_text, day_text, strlen(day_text));
				}


				text_layer_set_text(date_layer, month_text);
				text_layer_set_text(Weekday_Layer, weekday_text); //Update the weekday layer	


	if (units_changed & MINUTE_UNIT) 
	{

			/*
			if (units_changed & DAY_UNIT)
			{	
			} // DAY CHANGES
			*/

			//Format the Local Time	
			if (clock_is_24h_style())
			{
				strftime(time_text, sizeof(time_text), "%H:%M", tick_time);
			}
			else
			{
				strftime(time_text, sizeof(time_text), "%I:%M", tick_time);
			}


  			text_layer_set_text(Time_Layer, time_text);


			//WORLD CLOCK
	/*
			static char tz1_name[]="Gurgaon";
			int tz1_hours= 4;
			int tz1_min=30;
			static char tz2_name[]="Buenos Aires";
			int tz2_hours= -4;
			int tz2_min=0;
			static char tz3_name[]="Chicago";
			int tz3_hours= -7;
			int tz3_min=0;
			static char tz4_name[]="San Francisco";
			int tz4_hours= -9;
			int tz4_min=0;

			static char TZ1[] = "00:00";
			static char TZ2[] = "00:00";
			static char TZ3[] = "00:00";
			static char TZ4[] = "00:00";
*/

		
		
			time_t actualPtr = time(NULL);

			//Define and Calculate Time Zones
			//TIME ZONE 1
				struct tm *tz1Ptr = gmtime(&actualPtr);
				tz1Ptr->tm_hour += tz1_hours;
				tz1Ptr->tm_min += tz1_min;
		
				//try to fix the timezone when half and hour diff
				if (tz1Ptr->tm_min >=60){
					tz1Ptr->tm_hour = 1 + tz1Ptr->tm_hour;
					tz1Ptr->tm_min = tz1Ptr->tm_min - 60;
				}

				//try to fix the timezone when negative
		
				if (tz1Ptr->tm_hour <0){
					tz1Ptr->tm_hour = 24 + tz1Ptr->tm_hour;
				}
		
				//try to fix the timezone when more than 24
				if (tz1Ptr->tm_hour >=24){
					tz1Ptr->tm_hour = tz1Ptr->tm_hour - 24;
				}

				if (clock_is_24h_style()){strftime(TZ1, sizeof(TZ1), "%H:%M", tz1Ptr);}
				else {strftime(TZ1, sizeof(TZ1), "%I:%M", tz1Ptr);}
			//TIME ZONE 2
				struct tm *tz2Ptr = gmtime(&actualPtr);
				tz2Ptr->tm_hour += tz2_hours;
				tz2Ptr->tm_min += tz2_min;
		
				//try to fix the timezone when half and hour diff
				if (tz2Ptr->tm_min >=60){
					tz2Ptr->tm_hour = 1 + tz2Ptr->tm_hour;
					tz2Ptr->tm_min = tz2Ptr->tm_min - 60;
				}

				//try to fix the timezone when negative
				if (tz2Ptr->tm_hour <0){
					tz2Ptr->tm_hour = 24 + tz2Ptr->tm_hour;
				}
		
				//try to fix the timezone when more than 24
				if (tz2Ptr->tm_hour >=24){
					tz2Ptr->tm_hour = tz2Ptr->tm_hour - 24;
				}

				if (clock_is_24h_style()){strftime(TZ2, sizeof(TZ2), "%H:%M", tz2Ptr);}
				else{strftime(TZ2, sizeof(TZ2), "%I:%M", tz2Ptr);}
			//TIME ZONE 3		
				struct tm *tz3Ptr = gmtime(&actualPtr);
				tz3Ptr->tm_hour += tz3_hours;
				tz3Ptr->tm_min += tz3_min;
		
				//try to fix the timezone when half and hour diff
				if (tz3Ptr->tm_min >=60){
					tz3Ptr->tm_hour = 1 + tz3Ptr->tm_hour;
					tz3Ptr->tm_min = tz3Ptr->tm_min - 60;
				}

				//try to fix the timezone when negative
				if (tz3Ptr->tm_hour <0){
					tz3Ptr->tm_hour = 24 + tz3Ptr->tm_hour;
				}

				//try to fix the timezone when more than 24
				if (tz3Ptr->tm_hour >=24){
					tz3Ptr->tm_hour = tz3Ptr->tm_hour - 24;
				}
		
				if (clock_is_24h_style()){strftime(TZ3, sizeof(TZ3), "%H:%M", tz3Ptr);}
				else{strftime(TZ3, sizeof(TZ3), "%I:%M", tz3Ptr);}
			//TIME ZONE 4
				struct tm *tz4Ptr = gmtime(&actualPtr);
				tz4Ptr->tm_hour += tz4_hours;
				tz4Ptr->tm_min += tz4_min;
		
				//try to fix the timezone when half and hour diff
				if (tz4Ptr->tm_min >=60){
					tz4Ptr->tm_hour = 1 + tz4Ptr->tm_hour;
					tz4Ptr->tm_min = tz4Ptr->tm_min - 60;
				}

				//try to fix the timezone when negative
				if (tz4Ptr->tm_hour <0){
					tz4Ptr->tm_hour = 24 + tz4Ptr->tm_hour;
				}

				//try to fix the timezone when more than 24
				if (tz4Ptr->tm_hour >=24){
					tz4Ptr->tm_hour = tz4Ptr->tm_hour - 24;
				}
		
				if (clock_is_24h_style()){strftime(TZ4, sizeof(TZ4), "%H:%M", tz4Ptr);}
				else{strftime(TZ4, sizeof(TZ4), "%I:%M", tz4Ptr);}

			//DISPLAY THE TIME ZONES	
			text_layer_set_text(WC1NAME_Layer, tz1_name);
			text_layer_set_text(WC1TIME_Layer, TZ1);

			text_layer_set_text(WC2NAME_Layer, tz2_name);
			text_layer_set_text(WC2TIME_Layer, TZ2);

			text_layer_set_text(WC3NAME_Layer, tz3_name);
			text_layer_set_text(WC3TIME_Layer, TZ3);

			text_layer_set_text(WC4NAME_Layer, tz4_name);
			text_layer_set_text(WC4TIME_Layer, TZ4);

			//Check Battery Status
			handle_battery(battery_state_service_peek());

			//Check BT Status
			handle_bluetooth(bluetooth_connection_service_peek());

	} //MINUTE CHANGES
} //HANDLE_TICK 



//****************************//
// Initialize the application //
//****************************//

void handle_init(void)
{
	//Define Resources
    ResHandle res_d;
	ResHandle res_u;
	ResHandle res_t;
	ResHandle res_temp;
	
	
		         // Setup messaging
                const int inbound_size = 256;
                const int outbound_size = 256;
	
                app_message_open(inbound_size, outbound_size);

                Tuplet initial_values[] = {
                TupletCString(TZ1Name_KEY, ""),
				TupletInteger(TZ1Hour_KEY, (int8_t) 0), 
                TupletInteger(TZ1Minute_KEY, (int8_t) 0),
				TupletCString(TZ2Name_KEY, ""),
				TupletInteger(TZ2Hour_KEY, (int8_t) 0), 
                TupletInteger(TZ2Minute_KEY, (int8_t) 0),
				TupletCString(TZ3Name_KEY, ""),
				TupletInteger(TZ3Hour_KEY, (int8_t) 0), 
                TupletInteger(TZ3Minute_KEY, (int8_t) 0),
				TupletCString(TZ4Name_KEY, ""),
				TupletInteger(TZ4Hour_KEY, (int8_t) 0), 
                TupletInteger(TZ4Minute_KEY, (int8_t) 0),
				TupletInteger(INVERT_COLOR_KEY, persist_read_bool(INVERT_COLOR_KEY)),
                }; //TUPLET INITIAL VALUES
        
                app_sync_init(&sync, sync_buffer, sizeof(sync_buffer), initial_values,
                ARRAY_LENGTH(initial_values), sync_tuple_changed_callback,
                NULL, NULL);
	
			// read saved settings
			persist_read_string(TZ1Name_KEY, tz1_name, sizeof(tz1_name));
			tz1_hours=persist_read_int(TZ1Hour_KEY);
			tz1_min=persist_read_int(TZ1Minute_KEY);
			persist_read_string(TZ2Name_KEY, tz2_name, sizeof(tz2_name));
			tz2_hours=persist_read_int(TZ2Hour_KEY);
			tz2_min=persist_read_int(TZ2Minute_KEY);
			persist_read_string(TZ3Name_KEY, tz3_name, sizeof(tz3_name));
			tz3_hours=persist_read_int(TZ3Hour_KEY);
			tz3_min=persist_read_int(TZ3Minute_KEY);
			persist_read_string(TZ4Name_KEY, tz4_name, sizeof(tz4_name));
			tz4_hours=persist_read_int(TZ4Hour_KEY);
			tz4_min=persist_read_int(TZ4Minute_KEY);

	//Create the main window
	my_window = window_create(); 
	window_stack_push(my_window, true /* Animated */);
	window_set_background_color(my_window, GColorBlack);


	//Load the custom fonts
	res_t = resource_get_handle(RESOURCE_ID_FUTURA_CONDENSED_53); // Time font
	res_d = resource_get_handle(RESOURCE_ID_FUTURA_17); // Date font
	res_u = resource_get_handle(RESOURCE_ID_FUTURA_14); // Last Update font
	//res_temp =  resource_get_handle(RESOURCE_ID_FUTURA_36); //Temperature


    font_date = fonts_load_custom_font(res_d);
	font_update = fonts_load_custom_font(res_u);
	font_time = fonts_load_custom_font(res_t);



	//LOAD THE LAYERS
		//Display the Weekday layer
		Weekday_Layer = text_layer_create(WEEKDAY_FRAME);
		text_layer_set_text_color(Weekday_Layer, GColorWhite);
		text_layer_set_background_color(Weekday_Layer, GColorClear);
		text_layer_set_font(Weekday_Layer, font_date);
		text_layer_set_text_alignment(Weekday_Layer, GTextAlignmentLeft);
		layer_add_child(window_get_root_layer(my_window), text_layer_get_layer(Weekday_Layer)); 

		//Display the Batt layer
		Batt_icon_layer = bitmap_layer_create(BATT_FRAME);
  		bitmap_layer_set_bitmap(Batt_icon_layer, Batt_image);
  		layer_add_child(window_get_root_layer(my_window), bitmap_layer_get_layer(Batt_icon_layer));

		//Display the BT layer
	  	BT_icon_layer = bitmap_layer_create(BT_FRAME);
  		bitmap_layer_set_bitmap(BT_icon_layer, BT_image);
  		layer_add_child(window_get_root_layer(my_window), bitmap_layer_get_layer(BT_icon_layer));

		//Display the Time layer
		Time_Layer = text_layer_create(TIME_FRAME);
		text_layer_set_text_color(Time_Layer, GColorWhite);
		text_layer_set_background_color(Time_Layer, GColorClear);
		text_layer_set_font(Time_Layer, font_time);
		text_layer_set_text_alignment(Time_Layer, GTextAlignmentCenter);
		layer_add_child(window_get_root_layer(my_window), text_layer_get_layer(Time_Layer)); 

		//Display the Date layer
		date_layer = text_layer_create(DATE_FRAME);
		text_layer_set_text_color(date_layer, GColorWhite);
		text_layer_set_background_color(date_layer, GColorClear);
		text_layer_set_font(date_layer, font_date);
		text_layer_set_text_alignment(date_layer, GTextAlignmentRight);
		layer_add_child(window_get_root_layer(my_window), text_layer_get_layer(date_layer)); 

		//Display the Work Clock 1 layer
		WC1NAME_Layer = text_layer_create(WC1NAME_FRAME);
		WC1TIME_Layer = text_layer_create(WC1TIME_FRAME);
		text_layer_set_text_color(WC1NAME_Layer, GColorWhite);
		text_layer_set_text_color(WC1TIME_Layer, GColorWhite);
		text_layer_set_background_color(WC1NAME_Layer, GColorClear);
		text_layer_set_background_color(WC1TIME_Layer, GColorClear);
		text_layer_set_font(WC1NAME_Layer, font_update);
		text_layer_set_font(WC1TIME_Layer, font_update);
		text_layer_set_text_alignment(WC1NAME_Layer, GTextAlignmentLeft);
		text_layer_set_text_alignment(WC1TIME_Layer, GTextAlignmentRight);
		layer_add_child(window_get_root_layer(my_window), text_layer_get_layer(WC1NAME_Layer)); 
		layer_add_child(window_get_root_layer(my_window), text_layer_get_layer(WC1TIME_Layer)); 

		//Display the Work Clock 2 layer
		WC2NAME_Layer = text_layer_create(WC2NAME_FRAME);
		WC2TIME_Layer = text_layer_create(WC2TIME_FRAME);
		text_layer_set_text_color(WC2NAME_Layer, GColorWhite);
		text_layer_set_text_color(WC2TIME_Layer, GColorWhite);
		text_layer_set_background_color(WC2NAME_Layer, GColorClear);
		text_layer_set_background_color(WC2TIME_Layer, GColorClear);
		text_layer_set_font(WC2NAME_Layer, font_update);
		text_layer_set_font(WC2TIME_Layer, font_update);
		text_layer_set_text_alignment(WC2NAME_Layer, GTextAlignmentLeft);
		text_layer_set_text_alignment(WC2TIME_Layer, GTextAlignmentRight);
		layer_add_child(window_get_root_layer(my_window), text_layer_get_layer(WC2NAME_Layer)); 
		layer_add_child(window_get_root_layer(my_window), text_layer_get_layer(WC2TIME_Layer)); 

		//Display the Work Clock 3 layer
		WC3NAME_Layer = text_layer_create(WC3NAME_FRAME);
		WC3TIME_Layer = text_layer_create(WC3TIME_FRAME);
		text_layer_set_text_color(WC3NAME_Layer, GColorWhite);
		text_layer_set_text_color(WC3TIME_Layer, GColorWhite);
		text_layer_set_background_color(WC3NAME_Layer, GColorClear);
		text_layer_set_background_color(WC3TIME_Layer, GColorClear);
		text_layer_set_font(WC3NAME_Layer, font_update);
		text_layer_set_font(WC3TIME_Layer, font_update);
		text_layer_set_text_alignment(WC3NAME_Layer, GTextAlignmentLeft);
		text_layer_set_text_alignment(WC3TIME_Layer, GTextAlignmentRight);
		layer_add_child(window_get_root_layer(my_window), text_layer_get_layer(WC3NAME_Layer)); 
		layer_add_child(window_get_root_layer(my_window), text_layer_get_layer(WC3TIME_Layer)); 

		//Display the Work Clock 4 layer
		WC4NAME_Layer = text_layer_create(WC4NAME_FRAME);
		WC4TIME_Layer = text_layer_create(WC4TIME_FRAME);
		text_layer_set_text_color(WC4NAME_Layer, GColorWhite);
		text_layer_set_text_color(WC4TIME_Layer, GColorWhite);
		text_layer_set_background_color(WC4NAME_Layer, GColorClear);
		text_layer_set_background_color(WC4TIME_Layer, GColorClear);
		text_layer_set_font(WC4NAME_Layer, font_update);
		text_layer_set_font(WC4TIME_Layer, font_update);
		text_layer_set_text_alignment(WC4NAME_Layer, GTextAlignmentLeft);
		text_layer_set_text_alignment(WC4TIME_Layer, GTextAlignmentRight);
		layer_add_child(window_get_root_layer(my_window), text_layer_get_layer(WC4NAME_Layer)); 
		layer_add_child(window_get_root_layer(my_window), text_layer_get_layer(WC4TIME_Layer)); 

	// Ensures time is displayed immediately (will break if NULL tick event accessed).
	  // (This is why it's a good idea to have a separate routine to do the update itself.)

		time_t now = time(NULL);
	  	struct tm *current_time = localtime(&now);
		handle_tick(current_time, MINUTE_UNIT);
		tick_timer_service_subscribe(MINUTE_UNIT, &handle_tick);

		//Enable the Battery check event
		battery_state_service_subscribe(&handle_battery);
		//Enable the Bluetooth check event
	 	bluetooth_connection_service_subscribe(&handle_bluetooth);

} //HANDLE_INIT



//**********************//
// Kill the application //
//**********************//
void handle_deinit(void)
{
  //text_layer_destroy(text_layer);

	//Unsuscribe services
	tick_timer_service_unsubscribe();
 	battery_state_service_unsubscribe();
  	bluetooth_connection_service_unsubscribe();

	if (BT_image) {gbitmap_destroy(BT_image);}
	if (Batt_image){gbitmap_destroy(Batt_image);}

	//Deallocate layers
	text_layer_destroy(Time_Layer);
	text_layer_destroy(date_layer);
	text_layer_destroy(Weekday_Layer);

	//Deallocate custom fonts
	fonts_unload_custom_font(font_date);
	fonts_unload_custom_font(font_update);
	fonts_unload_custom_font(font_time);

	//Deallocate the main window
  	window_destroy(my_window);

} //HANDLE_DEINIT


//*************//
// ENTRY POINT //
//*************//
int main(void) 
{	
	handle_init();
	app_event_loop();
	handle_deinit();
}