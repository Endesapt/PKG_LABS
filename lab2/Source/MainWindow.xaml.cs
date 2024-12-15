using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Drawing;
using System.Windows.Shapes;
using WinForms = System.Windows.Forms;
using System.IO;
using Microsoft.Win32;
using ImageMagick;
using System.Collections.ObjectModel;

namespace WPF_Image_Info
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public ObservableCollection<ImageInfo> FileInfoList=new ObservableCollection<ImageInfo>();
        public MainWindow()
        {
            
            InitializeComponent();
            dataGrid1.ItemsSource = FileInfoList;
            dataGrid1.IsReadOnly = true;
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            
            Microsoft.Win32.OpenFileDialog openFileDialog = new Microsoft.Win32.OpenFileDialog
            {
                Filter = "Image Files|*.jpg;*.jpeg;*.png;*.bmp;*.gif;*.tiff;*.pcx",
                Title = "Select Images",
                Multiselect = true
            };

            if (openFileDialog.ShowDialog() == true)
            {
                FileInfoList.Clear();
                foreach (string fileName in openFileDialog.FileNames)
                {
                    GetImageInfo(fileName);
                }
            }
        }
        private void button2_Click(object sender, RoutedEventArgs e)
        {
            FolderBrowserDialog dialog= new FolderBrowserDialog();
            if (dialog.ShowDialog() == WinForms.DialogResult.OK)
            {
                if (Directory.Exists(dialog.SelectedPath))
                {
                    try
                    {
                        string[] imageExtensions = { "*.jpg", "*.jpeg", "*.png", "*.bmp", "*.gif","*.tiff","*.pcx"};
                        var imageFiles = new List<string>();

                        foreach (var extension in imageExtensions)
                        {
                            imageFiles.AddRange(Directory.GetFiles(dialog.SelectedPath, extension));
                        }

                        FileInfoList.Clear();
                        foreach (var file in imageFiles)
                        {
                            GetImageInfo(file);
                        }
                    }
                    catch (Exception ex)
                    {
                        System.Windows.MessageBox.Show($"Error: {ex.Message}");
                    }
                }
            }
        }
        private void GetImageInfo(string filePath)
        {
            if (!File.Exists(filePath))
            {
                System.Windows.MessageBox.Show("Файл не найден.");
                return;
            }

            
            MagickImage image = new MagickImage(filePath);
            //BitmapImage bitmap = new BitmapImage(new Uri(filePath));

            // Имя файла
            string fileName = System.IO.Path.GetFileName(filePath);

            // Размер изображения в пикселях
            int width = (int)image.Width;
            int height = (int)image.BaseHeight;
            string compression = image.Compression.ToString();
            double DensityX = image.Density.X;
            double DensityY = image.Density.Y;

            if (DensityX == 0 || DensityY==0)
            {
                BitmapImage bitmap = new BitmapImage(new Uri(filePath));
                DensityX = bitmap.DpiX;
                DensityY = bitmap.DpiY;
            }

            var fileInfo = new ImageInfo
            {
                Name = fileName,
                Size = $"{width}x{height}",
                Resolution=$"{DensityX}x{DensityY}",
                ColorDepth=image.Depth,
                Compression=compression,
            };
            FileInfoList.Add(fileInfo);


        }

        
    }
    public class ImageInfo
    {
        public required string Name { get; set; }
        public required string Size { get; set; }
        public required string Resolution {  get; set; }
        public required uint ColorDepth { get; set; }
        public required string Compression { get; set; } 
    }
}