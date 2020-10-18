#include <vector>

#include <libmail/include/send_mail.h>
#include <libmail/include/randomize_results.h>


int main()
{
	libmail::randomize_results();
	std::vector<std::string> responsible_devs = {
		"dev1@company.com",
		"dev2@company.com",
		"dev3@company.com",
		"dev4@company.com",
		"dev5@company.com",
	};
	std::string message = "build failed";

	// ...
}
