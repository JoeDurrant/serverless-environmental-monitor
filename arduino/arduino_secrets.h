// Fill in  your WiFi networks SSID and password
#define SECRET_SSID "Put the name of your WiFi network here"
#define SECRET_PASS "Wifi password"

// Fill in the hostname of your AWS IoT broker
#define SECRET_BROKER "xxxxxxxxxxxxxxxxxx.iot.us-east-1.amazonaws.com"

// Fill in the boards public certificate
const char SECRET_CERTIFICATE[] = R"(
-----BEGIN CERTIFICATE REQUEST-----
//Place your certificate here with no line breaks
-----END CERTIFICATE REQUEST-----
)";
