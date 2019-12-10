//
// send email with CHILKAT library
// https://www.example-code.com/cpp/smtp_simpleSend.asp
// 

#include <iostream>
#include <CkMailMan.h>
#include <CkEmail.h>
#include <CkGlobal.h>
using namespace std;

#pragma comment(lib, "crypt32.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "dnsapi.lib")
#pragma comment(lib, "ChilkatDbg.lib")

void ChilkatSample()
{
	// https://www.example-code.com/cpp/global_unlock.asp
	// The Chilkat API can be unlocked for a fully-functional 30-day trial by passing any
	// string to the UnlockBundle method.  A program can unlock once at the start. Once unlocked,
	// all subsequently instantiated objects are created in the unlocked state. 
	// 
	// After licensing Chilkat, replace the "Anything for 30-day trial" with the purchased unlock code.
	// To verify the purchased unlock code was recognized, examine the contents of the LastErrorText
	// property after unlocking.  For example:
	CkGlobal glob;
	bool success = glob.UnlockBundle("Anything for 30-day trial");
	if (success != true) {
		std::cout << glob.lastErrorText() << "\r\n";
		return;
	}

	int status = glob.get_UnlockStatus();
	if (status == 2) {
		std::cout << "Unlocked using purchased unlock code." << "\r\n";
	}
	else {
		std::cout << "Unlocked in trial mode." << "\r\n";
	}

	// The LastErrorText can be examined in the success case to see if it was unlocked in
	// trial more, or with a purchased unlock code.
	std::cout << glob.lastErrorText() << "\r\n";


	// This example requires the Chilkat API to have been previously unlocked.
	// See Global Unlock Sample for sample code.

	// The mailman object is used for sending and receiving email.
	CkMailMan mailman;

	// Set the SMTP server.
	mailman.put_SmtpHost("smtp.gmail.com");

	// Set the SMTP login/password (if required)
	mailman.put_SmtpUsername("jjuiddong@gmail.com");
	mailman.put_SmtpPassword("");

	// Create a new email object
	CkEmail email;

	email.put_Subject("This is a test");
	email.put_Body("This is a test");
	email.put_From("Chilkat Support <jjuiddong@gmail.com>");
	success = email.AddTo("jjuiddong", "jjuiddong@hanmail.net");
	// To add more recipients, call AddTo, AddCC, or AddBcc once per recipient.

	// Call SendEmail to connect to the SMTP server and send.
	// The connection (i.e. session) to the SMTP server remains
	// open so that subsequent SendEmail calls may use the
	// same connection.  
	success = mailman.SendEmail(email);
	if (success != true) {
		std::cout << mailman.lastErrorText() << "\r\n";
		return;
	}

	// Some SMTP servers do not actually send the email until 
	// the connection is closed.  In these cases, it is necessary to
	// call CloseSmtpConnection for the mail to be  sent.  
	// Most SMTP servers send the email immediately, and it is 
	// not required to close the connection.  We'll close it here
	// for the example:
	success = mailman.CloseSmtpConnection();
	if (success != true) {
		std::cout << "Connection to SMTP server not closed cleanly." << "\r\n";
	}

	std::cout << "Mail Sent!" << "\r\n";
}


int main()
{
	ChilkatSample();
	return 1;
}
