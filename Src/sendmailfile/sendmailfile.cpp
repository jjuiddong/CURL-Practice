//
// send mail with file sample code with curl library
// refrence
//  - https://curl.haxx.se/libcurl/c/smtp-tls.html
//
#include <iostream>
#include <string>
#include <fstream>
#include <stdio.h>
#include <curl/curl.h>
#pragma comment(lib, "libcurl.lib")

#define FROM    "<jjuiddong@gmail.com>"
#define TO      "<jjuiddong@hanmail.net>"
#define CC      "<jjuiddong@naver.com>"
#define FILENAME "image.jpg"

static const char *payload_text[] = {
  "Date: Mon, 29 Nov 2010 21:54:29 +1100\r\n",
  "To: " TO "\r\n",
  "From: " FROM " (jjuiddong)\r\n",
  "Cc: " CC " (Another example User)\r\n",
  "Message-ID: <dcd7cb36-11db-487a-9f3a-e652a9458efd@"
  "rfcpedant.example.org>\r\n",
  "Subject: Send Email With Attachment File8\r\n",
  "\r\n", /* empty line to divide headers from body, see RFC5322 */
  "The body of the message starts here.\r\n",
  "\r\n",
  "It could be a lot of lines, could be MIME encoded, whatever.\r\n",
  "Check RFC5322.\r\n",
  "\r\n",
  "http://jjuiddong.iptime.org/img.zip",
  NULL
};

struct upload_status {
	int lines_read;
};

static size_t payload_source(void *ptr, size_t size, size_t nmemb, void *userp)
{
	struct upload_status *upload_ctx = (struct upload_status *)userp;
	const char *data;
	if ((size == 0) || (nmemb == 0) || ((size*nmemb) < 1)) {
		return 0;
	}
	data = payload_text[upload_ctx->lines_read];
	if (data) {
		size_t len = strlen(data);
		memcpy(ptr, data, len);
		upload_ctx->lines_read++;
		return len;
	}
	return 0;
}


int SendMail(const char *from, const char **rcpts)
{
	CURL *curl;
	CURLcode res = CURLE_OK;
	struct curl_slist *recipients = NULL;
	struct upload_status upload_ctx;
	upload_ctx.lines_read = 0;

	int file_size = 0;
	{
		struct __stat64 buf;
		_stat64(FILENAME, &buf);
		file_size = (int)buf.st_size;
	}	

	curl = curl_easy_init();
	if (curl)
	{
		//curl_easy_setopt(curl, CURLOPT_URL, "smtp://mx1.hanmail.net");
		curl_easy_setopt(curl, CURLOPT_USERNAME, "jjuiddong");
		curl_easy_setopt(curl, CURLOPT_PASSWORD, "");
		curl_easy_setopt(curl, CURLOPT_URL, "smtp.gmail.com:587");
		curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);
		curl_easy_setopt(curl, CURLOPT_CAINFO, "cacert.pem");
		curl_easy_setopt(curl, CURLOPT_MAIL_FROM, from);

		int i = 0;
		while (rcpts[i])
		{
			recipients = curl_slist_append(recipients, rcpts[i]);
			i++;
		}

		curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);
		curl_easy_setopt(curl, CURLOPT_READFUNCTION, payload_source);
		curl_easy_setopt(curl, CURLOPT_READDATA, &upload_ctx);
		//curl_easy_setopt(curl, CURLOPT_INFILESIZE, file_size);
		curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
		res = curl_easy_perform(curl);

		if (res != CURLE_OK)
		{
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
				curl_easy_strerror(res));
		}

		curl_slist_free_all(recipients);
		curl_easy_cleanup(curl);
	}

	return (int)res;
}


int main(int argc, char *argv[])
{
	const char *from = "jjuiddong@gmail.com";
	const char *recipients[] = { "jjuiddong@hanmail.net", NULL, "jjuiddong@gmail.com", NULL };

	int ret = SendMail(from, recipients);
	printf("ret %d\n", ret);

	return EXIT_SUCCESS;
}
