/* imontty
   Show status of ISDN lines of isdn4linux

   (c) 1995 Volker Goetz
*/

#include <linux/isdn.h>
#include <iostream.h>
#include <fstream.h>
#include <String.h>
#include <std.h>
#include <GetOpt.h>

int print_isdnstatus(ifstream & isdninfo)
{
   int i;
   // The contents of the info device, currently 5 lines
   String Info[6];
 

   // read 6 lines and get all the necessary info
   for (i = 0; i < 6; i++)
      readline(isdninfo, Info[i]);

   String idmap[ISDN_MAX_CHANNELS];
   String chmap[ISDN_MAX_CHANNELS];
   String drmap[ISDN_MAX_CHANNELS];
   String usage[ISDN_MAX_CHANNELS];
   String flags[ISDN_MAX_CHANNELS];
   String phone[ISDN_MAX_CHANNELS];
   
   // Now do some conversions...

   String separator=" ";
   int count;

   // split output of isdninfo into entries of an array

   count = split(Info[0].after(6), idmap, ISDN_MAX_CHANNELS, separator);
   count = split(Info[1].after(6), chmap, ISDN_MAX_CHANNELS, separator);
   count = split(Info[2].after(6), drmap, ISDN_MAX_CHANNELS, separator);
   count = split(Info[3].after(6), usage, ISDN_MAX_CHANNELS, separator);
   count = split(Info[4].after(6), flags, ISDN_MAX_CHANNELS, separator);
   count = split(Info[5].after(6), phone, ISDN_MAX_CHANNELS, separator);

   int i_chmap[ISDN_MAX_CHANNELS];
   int i_drmap[ISDN_MAX_CHANNELS];
   int i_usage[ISDN_MAX_CHANNELS];
   int i_flags[ISDN_MAX_CHANNELS];
   int i_phone[ISDN_MAX_CHANNELS];

   
   // convert some of the entries from string to integer

   for (i = 0; i < count; i++)
      i_chmap[i] = atoi( (const char*)chmap[i] );
   
   for (i = 0; i < count; i++)
      i_drmap[i] = atoi( (const char*)drmap[i]);
   
   for (i = 0; i < count; i++)
      i_usage[i] = atoi( (const char*)usage[i]);
   

   // print out actual status of all available channels

   int usage_akt;
  
   cout << endl << "ISDN channel status:" << endl << endl;
   cout << "Ch. #   DriverId   Direction\tUsage\t\tNumber" << endl;
   cout << "-----------------------------------------------------------------------" << endl;
   
   for (i = 0; i < count; i++)
   {
      // check whether channel is available
      if ( (i_chmap[i] >= 0) && (i_drmap[i] >= 0))
      {
	 cout << i << "\t" << idmap[i] << "\t" ;
	 usage_akt = i_usage[i];
	 
	 if (usage_akt > 128)
	 {
	    cout << "outgoing\t";
	    usage_akt = usage_akt - 128;
	 }
	 else
	 {
	    if ((usage_akt&7) > 0)
	       cout << "incoming\t";
	    else
	    {
	       if ((usage_akt&64) > 0)
	 	   cout << "Excl.\t";
	       else
	           cout << "free\t";
            }
	 }
	 
	 switch (usage_akt&7)
	 {
	    case 5: cout << "fax\t\t"; break;
	    case 4: cout << "voice\t\t"; break;
	    case 3: cout << "IP interface\t"; break;
	    case 2: cout << "tty emulation\t"; break;
	    case 1: cout << "raw device\t"; break;
	    case 0: cout << "--\t\t"; break;
	    default: cout << "unknown\t"; break;
	 }
	 
	 cout << phone[i] << endl;
      } // if
   } // for i
   cout << endl;


   return 0;
}


int main(int argc, char **argv)
{

   // some default settings

   // the name of the isdn info device
   String infodevice="/dev/isdninfo";

   // do no continuous output
   int continuous_flag = 0;

   // Command line options
   // -f      - do continuous output
   // -i dev  - isdn info device

   const String options = "fi:";

   // call GetOpt constructor
   GetOpt getopt (argc, argv, options);
   int option_char;

   // get arguments
   while ( ( option_char = getopt() ) != EOF )
      switch (option_char)
      {
	 case 'i' : infodevice = getopt.optarg; break;
	 case 'f' : continuous_flag = 1; break;
	 case '?' : cerr << endl << "Usage: " << argv[0] << " [-i dev] [-f [-d sec]]" << endl << endl;
	    cerr << "Options:" << endl;
	    cerr << "-i dev : ISDN info device (default /dev/isdninfo)" << endl;
	    cerr << "-f     : continuous output" << endl;
	    return 1;	    
      }
   
   // open the info device and check if ok

   ifstream isdninfo(infodevice);
   if (!isdninfo)
   {
      cerr << "ERROR: cannot open ISDN info device " << infodevice << endl;
      return 1;
   }
   isdninfo.unsetf(ios::skipws);      // don't skip white space

   if ( continuous_flag )
   {
      int printerror = 0;
      
      while ( !printerror )
      {
      	 // clear screen
	 // quite bad since "hard coded" escape sequence
	 
	 cout << "\033[1;1H\033[J";
         printerror = print_isdnstatus(isdninfo);
	 cout << "Stop by pressing Ctrl-C" << endl;
      }
   }
   else
         print_isdnstatus(isdninfo);
   
   // and close the input stream
   isdninfo.close();
   
}
