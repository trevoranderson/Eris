using System;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Foundation;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using System.Threading.Tasks;
using Windows.Web.Http;
using Windows.Data.Json;
using Newtonsoft.Json;
using Windows.Web.Http.Headers;
using System.Text;
using Windows.Storage.Streams;
using System.Net.Http.Headers;
using System.Net;
using System.Collections.Generic;
using Windows.UI.Xaml.Media.Imaging;
using System.IO;
using Windows.UI.Xaml.Data;
using Windows.System.Threading;
using System.Threading;
using System.ComponentModel;
using Windows.UI.Xaml.Media;
using Windows.Web.Http.Filters;
using Windows.UI.Core;
// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=234238

namespace windowseris
{
    class HttpJsonContent : IHttpContent
    {
        IJsonValue jsonValue;
        HttpContentHeaderCollection headers;

        public HttpContentHeaderCollection Headers
        {
            get
            {
                return headers;
            }
        }

        public HttpJsonContent(IJsonValue jsonValue)
        {
            if (jsonValue == null)
            {
                throw new ArgumentException("jsonValue cannot be null.");
            }
            this.jsonValue = jsonValue;
            headers = new HttpContentHeaderCollection();
            headers.ContentType = new HttpMediaTypeHeaderValue("application/json");
            headers.ContentType.CharSet = "UTF-8";
        }

        public IAsyncOperationWithProgress<ulong, ulong> BufferAllAsync()
        {
            return AsyncInfo.Run<ulong, ulong>((cancellationToken, progress) =>
            {
                return Task<ulong>.Run(() =>
                {
                    ulong length = GetLength();

                    // Report progress.
                    progress.Report(length);

                    // Just return the size in bytes.
                    return length;
                });
            });
        }

        public IAsyncOperationWithProgress<IBuffer, ulong> ReadAsBufferAsync()
        {
            return AsyncInfo.Run<IBuffer, ulong>((cancellationToken, progress) =>
            {
                return Task<IBuffer>.Run(() =>
                {
                    DataWriter writer = new DataWriter();
                    writer.WriteString(jsonValue.Stringify());

                    // Make sure that the DataWriter destructor does not free the buffer.
                    IBuffer buffer = writer.DetachBuffer();

                    // Report progress.
                    progress.Report(buffer.Length);

                    return buffer;
                });
            });
        }

        public IAsyncOperationWithProgress<IInputStream, ulong> ReadAsInputStreamAsync()
        {
            return AsyncInfo.Run<IInputStream, ulong>(async (cancellationToken, progress) =>
            {
                InMemoryRandomAccessStream randomAccessStream = new InMemoryRandomAccessStream();
                DataWriter writer = new DataWriter(randomAccessStream);
                writer.WriteString(jsonValue.Stringify());

                uint bytesStored = await writer.StoreAsync().AsTask(cancellationToken);

                // Make sure that the DataWriter destructor does not close the stream.
                writer.DetachStream();

                // Report progress.
                progress.Report(randomAccessStream.Size);

                return randomAccessStream.GetInputStreamAt(0);
            });
        }

        public IAsyncOperationWithProgress<string, ulong> ReadAsStringAsync()
        {
            return AsyncInfo.Run<string, ulong>((cancellationToken, progress) =>
            {
                return Task<string>.Run(() =>
                {
                    string jsonString = jsonValue.Stringify();

                    // Report progress (length of string).
                    progress.Report((ulong)jsonString.Length);

                    return jsonString;
                });
            });
        }

        public bool TryComputeLength(out ulong length)
        {
            length = GetLength();
            return true;
        }

        public IAsyncOperationWithProgress<ulong, ulong> WriteToStreamAsync(IOutputStream outputStream)
        {
            return AsyncInfo.Run<ulong, ulong>(async (cancellationToken, progress) =>
            {
                DataWriter writer = new DataWriter(outputStream);
                writer.WriteString(jsonValue.Stringify());
                uint bytesWritten = await writer.StoreAsync().AsTask(cancellationToken);

                // Make sure that DataWriter destructor does not close the stream.
                writer.DetachStream();

                // Report progress.
                progress.Report(bytesWritten);

                return bytesWritten;
            });
        }

        public void Dispose()
        {
        }

        private ulong GetLength()
        {
            DataWriter writer = new DataWriter();
            writer.WriteString(jsonValue.Stringify());

            IBuffer buffer = writer.DetachBuffer();
            return buffer.Length;
        }
    }
    public class MainViewModel : INotifyPropertyChanged
    {
        public event PropertyChangedEventHandler PropertyChanged;

        private byte[] imageBuffer;
        public byte[] ImageBuffer
        {
            get { return imageBuffer; }
            set
            {
                imageBuffer = value;
                var loadImageTask = LoadImageAsync();
            }
        }

        private ImageSource imageSource;
        public ImageSource ImageSource
        {
            get { return imageSource; }
            set
            {
                imageSource = value;
                if (PropertyChanged != null)
                    PropertyChanged(this, new PropertyChangedEventArgs("ImageSource"));
            }
        }

        private async Task LoadImageAsync()
        {
            using (InMemoryRandomAccessStream ms = new InMemoryRandomAccessStream())
            {
                // Writes the image byte array in an InMemoryRandomAccessStream
                // that is needed to set the source of BitmapImage.
                using (DataWriter writer = new DataWriter(ms.GetOutputStreamAt(0)))
                {
                    writer.WriteBytes(imageBuffer);
                    await writer.StoreAsync();
                }

                var image = new BitmapImage();
                await image.SetSourceAsync(ms);
                ImageSource = image;
            }
        }
    }
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class MainPage : Page
    {
        MainViewModel m;
        public MainPage()
        {
            this.InitializeComponent();
            m = new MainViewModel();
            test();
        }
        public Int64 MilliTimeStamp()
        {
            DateTime d1 = new DateTime(1970, 1, 1);
            DateTime d2 = DateTime.UtcNow;
            TimeSpan ts = new TimeSpan(d2.Ticks - d1.Ticks);

            return (long)ts.TotalMilliseconds;
        }

        public async void send_input(string moveDir)
        {
            try
            {
                var httpClient = new Windows.Web.Http.HttpClient();
                var xauth = ":97f0ad9e24ca5e0408a269748d7fe0a0";
                var xauthbytes = Encoding.UTF8.GetBytes(xauth);
                var xauthbase64 = Convert.ToBase64String(xauthbytes);
                var xauthFinal = new HttpCredentialsHeaderValue("X-Auth-Token", xauthbase64);
                httpClient.DefaultRequestHeaders.Add("X-Auth-Token", xauth);

                long unixTimestamp = MilliTimeStamp();
                var sendObj = new
                {
                    time = unixTimestamp+6500,
                    move = moveDir,
                };
                string toSend = JsonConvert.SerializeObject(sendObj);
                IHttpContent moveData = new HttpJsonContent(JsonValue.Parse(toSend));
                HttpResponseMessage ErisResponse =
                    await httpClient.PostAsync(
                        new Uri("http://erisgaming.cloudapp.net:3000/collectionapi/inputqueue/")
                        , moveData);
                if (ErisResponse.IsSuccessStatusCode)
                {

                }
                else
                {

                }

            }
            catch (Exception send)
            {

            }
        }
        public async void SetImageFromByteArray(byte[] data)
        {
            using (InMemoryRandomAccessStream raStream =
                new InMemoryRandomAccessStream())
            {
                using (DataWriter writer = new DataWriter(raStream))
                {
                    // Write the bytes to the stream
                    writer.WriteBytes(data);

                    // Store the bytes to the MemoryStream
                    await writer.StoreAsync();

                    // Not necessary, but do it anyway
                    await writer.FlushAsync();

                    // Detach from the Memory stream so we don't close it
                    writer.DetachStream();
                }

                raStream.Seek(0);

                BitmapImage bitMapImage = new BitmapImage();
                bitMapImage.SetSource(raStream);
                GameScreen.Source = bitMapImage;
                await raStream.FlushAsync();
            }
        }
        private async Task<BitmapImage> ByteArrayToBitmapImage(byte[] byteArray)
        {
            var bitmapImage = new BitmapImage();

            var stream = new InMemoryRandomAccessStream();
            await stream.WriteAsync(byteArray.AsBuffer());
            stream.Seek(0);

            bitmapImage.SetSource(stream);
            return bitmapImage;
        }

        public async void setScreen()
        {

            var RootFilter = new HttpBaseProtocolFilter();

            RootFilter.CacheControl.ReadBehavior = Windows.Web.Http.Filters.HttpCacheReadBehavior.MostRecent; RootFilter.CacheControl.WriteBehavior = Windows.Web.Http.Filters.HttpCacheWriteBehavior.NoCache;

            //HttpClient = new HttpClient(RootFilter)
            var httpClient = new Windows.Web.Http.HttpClient(RootFilter);
            var xauth = ":97f0ad9e24ca5e0408a269748d7fe0a0";
            var xauthbytes = Encoding.UTF8.GetBytes(xauth);
            var xauthbase64 = Convert.ToBase64String(xauthbytes);
            var xauthFinal = new HttpCredentialsHeaderValue("X-Auth-Token", xauthbase64);
            httpClient.DefaultRequestHeaders.Add("X-Auth-Token", xauth);

            HttpResponseMessage ErisResponse =
                await httpClient.GetAsync(new Uri("http://erisgaming.cloudapp.net:3000/collectionapi/screen"));

            if (ErisResponse.IsSuccessStatusCode)
            {

                var a = ErisResponse.Content;
                var b = a.ToString();
                var dummy = new[] 
                    { new 
                        {
                            bmp = "b",
                        }
                    };
                var c = JsonConvert.DeserializeAnonymousType(b, dummy);
                string d = c[0].bmp;
                var e = d.Split(',')[1];


                var bytes = Convert.FromBase64String(e);
                var z = await ByteArrayToBitmapImage(bytes);
                GameScreen.Source = z;
                //SetImageFromByteArray(bytes);

            }
            else
            {

            }

        }


        private void render_image(object sender, RoutedEventArgs e)
        {
            setScreen();
        }

        private void left_input(object sender, RoutedEventArgs e)
        {
            send_input("left");
        }

        private void up_input(object sender, RoutedEventArgs e)
        {
            send_input("up");
        }

        private void down_input(object sender, RoutedEventArgs e)
        {
            send_input("down");
        }

        private void right_input(object sender, RoutedEventArgs e)
        {
            send_input("right");
        }

        private void start_input(object sender, RoutedEventArgs e)
        {
            send_input("start");
        }

        private void b_input(object sender, RoutedEventArgs e)
        {
            send_input("b");
        }

        private void a_input(object sender, RoutedEventArgs e)
        {
            send_input("a");
        }

        private void select_input(object sender, RoutedEventArgs e)
        {
            send_input("select");
        }

        public void test()
        {
            var period = TimeSpan.FromMilliseconds(100);
            //       .FromMinutes(5);

            var timer = ThreadPoolTimer.CreatePeriodicTimer(async (source) =>
            {
                //do your query/work here
                HttpResponseMessage ErisResponse= new HttpResponseMessage();
                try
                {
                    var RootFilter = new HttpBaseProtocolFilter();

                    RootFilter.CacheControl.ReadBehavior = Windows.Web.Http.Filters.HttpCacheReadBehavior.MostRecent; RootFilter.CacheControl.WriteBehavior = Windows.Web.Http.Filters.HttpCacheWriteBehavior.NoCache;

                    //HttpClient = new HttpClient(RootFilter)
                    var httpClient = new Windows.Web.Http.HttpClient(RootFilter);
                    var xauth = ":97f0ad9e24ca5e0408a269748d7fe0a0";
                    var xauthbytes = Encoding.UTF8.GetBytes(xauth);
                    var xauthbase64 = Convert.ToBase64String(xauthbytes);
                    var xauthFinal = new HttpCredentialsHeaderValue("X-Auth-Token", xauthbase64);
                    httpClient.DefaultRequestHeaders.Add("X-Auth-Token", xauth);

                    ErisResponse =
                       await httpClient.GetAsync(new Uri("http://erisgaming.cloudapp.net:3000/collectionapi/screen"));
                }
                catch (Exception a)
                {

                }

                // end query stuff
                Dispatcher.RunAsync(CoreDispatcherPriority.High,
                async () =>
                {
                    try
                    {
                        if (ErisResponse.IsSuccessStatusCode)
                        {

                            var a = ErisResponse.Content;
                            var b = a.ToString();
                            var dummy = new[] 
                    { new 
                        {
                            bmp = "b",
                        }
                    };
                            var c = JsonConvert.DeserializeAnonymousType(b, dummy);
                            string d = c[0].bmp;
                            var e = d.Split(',')[1];


                            var bytes = Convert.FromBase64String(e);
                            var z = await ByteArrayToBitmapImage(bytes);
                            GameScreen.Source = z;
                            //SetImageFromByteArray(bytes);

                        }
                        else
                        {

                        }
                    }
                    catch(Exception b)
                    { }
                    // Now that work is done, safely access UI components here to update them
                });
            }, period);
        }
    }
}
