#include "opencv4/opencv2/highgui/highgui.hpp"
#include "opencv4/opencv2/imgproc/imgproc.hpp"
#include "opencv4/opencv2/objdetect/objdetect.hpp"

#include <iostream>
#include <string>
#include <curl/curl.h>

void addHttpHeaders(CURL *curl, char const *headerAuthorization, char const *headerContentType)
{
    struct curl_slist *headerlist = NULL;
    headerlist = curl_slist_append(headerlist, headerAuthorization);
    headerlist = curl_slist_append(headerlist, headerContentType);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
}

CURLcode sendNotification(CURL *curl, std::string message, std::string file_name)
{
    std::string url = "https://notify-api.line.me/api/notify";
    const char *completeUrl = url.c_str();
    curl_easy_setopt(curl, CURLOPT_URL, completeUrl);

    curl_mime *form = NULL;
    curl_mimepart *field = NULL;

    form = curl_mime_init(curl);

    field = curl_mime_addpart(form);
    curl_mime_name(field, "message");
    curl_mime_data(field, message.c_str(), CURL_ZERO_TERMINATED);

    field = curl_mime_addpart(form);
    curl_mime_name(field, "imageFile");
    curl_mime_filedata(field, file_name.c_str());

    curl_easy_setopt(curl, CURLOPT_MIMEPOST, form);

    return curl_easy_perform(curl);
}

int main(int argc, char const *argv[])
{
    cv::Mat img = cv::imread("blackpink.jpg", cv::IMREAD_GRAYSCALE);
    cv::Mat face_roi;
    char name[20];

    CURL *curl;
    CURLcode res;
    std::string readBuffer;
    long responsecode;

    const char *headerContentType = "Content-Type: multipart/form-data";
    const char *headerAuthorization = "Authorization: Bearer YOUR-ACCESS-TOKEN";

    curl = curl_easy_init();
    if (curl) 
    {
        addHttpHeaders(curl, headerAuthorization, headerContentType);

        cv::CascadeClassifier face_detector("haarcascade_frontalface_alt2.xml");

        std::vector<cv::Rect> faces;
        face_detector.detectMultiScale(img, faces);

        for (size_t i = 0; i < faces.size(); i++)
        {
            img(faces[i]).copyTo(face_roi);

            sprintf(name, "face_%d.png", static_cast<int>(i));

            cv::rectangle(img, faces[i], cv::Scalar(255));

            cv::imwrite(std::string(name), face_roi);

            res = sendNotification(curl, "Face Detected: " + std::string(name), name);
        }
        
        if (res != CURLE_OK)
        {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }
        else 
        {
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &responsecode);
            if (responsecode == 200) {
                std::cout << " Success" << std::endl;
            } else if (responsecode == 400) {
                std::cout << " Bad request" << std::endl;
            } else if (responsecode == 401) {
                std::cout << " Inavalid access token" << std::endl;
            } else if (responsecode == 500) {
                std::cout << " Proccessed over time or stopped. Server answer with status code: " << responsecode << std::endl;
            }
        }

        curl_easy_cleanup(curl);
        std::cout << readBuffer << std::endl;

        cv::imshow("Image", img);
        cv::waitKey();
    }
    return 0;
}