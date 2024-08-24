const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
  <head>
    <!-- Meta tag for responsive design to ensure the webpage scales well on different devices -->
    <meta name="viewport" content="width=device-width, initial-scale=1">
    
    <!-- Link to Font Awesome for icons used in the webpage -->
    <link href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" rel="stylesheet">
    
    <style>
      /* Global styles for the HTML document */
      html {
        font-family: Arial, sans-serif;   /* Set default font */
        display: inline-block;            /* Inline-block allows multiple elements to sit next to each other */
        margin: 0px auto;                 /* Center the content horizontally */
        text-align: center;               /* Align text to the center */
      }

      /* Style for the body of the document */
      body {
        background-color: #f2f2f2;        /* Light grey background */
        color: #333;                      /* Dark grey text */
      }

      /* Style for the heading (h1) */
      h1 { 
        font-size: 2.5rem;                /* Large font size for the heading */
        color: #333;                      /* Dark grey color */
        margin-bottom: 30px;              /* Add space below the heading */
      }

      /* Style for paragraphs (p) */
      p { 
        font-size: 1.8rem;                /* Large font size for text */
      }

      /* Style for units such as °C or % */
      .units { 
        font-size: 1.2rem;                /* Smaller font size for units */
        color: #888;                      /* Light grey color */
      }

      /* Style for each sensor box that displays temperature, humidity, and heat index */
      .sensor-box {
        padding: 20px;                    /* Add padding inside the box */
        margin: 15px;                     /* Add margin outside the box */
        background-color: #fff;           /* White background for the box */
        box-shadow: 0px 0px 10px rgba(0, 0, 0, 0.1);  /* Light shadow around the box */
        border-radius: 10px;              /* Rounded corners */
        display: inline-block;            /* Display boxes inline with each other */
        width: 30%;                       /* Each box takes 30% of the container's width */
        vertical-align: top;              /* Align boxes to the top */
      }

      /* Style for the icons inside each sensor box */
      .icon {
        font-size: 3.0rem;                /* Large icon size */
        margin-bottom: 15px;              /* Add space below the icon */
      }

      /* Specific color for the temperature icon */
      .temperature-icon {
        color: #f39c12;                   /* Orange color */
      }

      /* Specific color for the humidity icon */
      .humidity-icon {
        color: #3498db;                   /* Blue color */
      }

      /* Specific color for the heat index icon */
      .heatindex-icon {
        color: #e74c3c;                   /* Red color */
      }
    </style>
  </head>

  <body>
    <!-- Main heading of the webpage -->
    <h1>Temperature, Humidity and Heat Monitoring System</h1>
    
    <!-- Sensor box for displaying temperature -->
    <div class="sensor-box">
      <div><i class="fas fa-thermometer-half icon temperature-icon"></i></div>  <!-- Temperature icon -->
      <p>Temperature: <span id="temperature">0.00</span> <span class="units">°C</span></p>  <!-- Temperature value -->
    </div>

    <!-- Sensor box for displaying humidity -->
    <div class="sensor-box">
      <div><i class="fas fa-tint icon humidity-icon"></i></div>  <!-- Humidity icon -->
      <p>Humidity: <span id="humidity">0.00</span> <span class="units">%</span></p>  <!-- Humidity value -->
    </div>

    <!-- Sensor box for displaying heat index -->
    <div class="sensor-box">
      <div><i class="fas fa-sun icon heatindex-icon"></i></div>  <!-- Heat Index icon -->
      <p>Heat Index: <span id="heatindex">0.00</span> <span class="units">°C</span></p>  <!-- Heat Index value -->
    </div>

    <!-- Section for displaying the latest calculation results (min, max, avg) -->
    <div>
      <h3>Latest Calculations (1-minute interval):</h3>
      <p id="calculationResults"></p>  <!-- Placeholder for calculation results -->
    </div>
  </body>

  <script>
    /* 
    JavaScript to periodically fetch sensor data and display it on the webpage 
    setInterval() is used to call getData() function every 1000 milliseconds (1 second)
    */
    setInterval(function () {
      getData("readTemperature", "temperature");             // Get temperature data from the server
      getData("readHumidity", "humidity");                   // Get humidity data from the server
      getData("readHeatIndex", "heatindex");                 // Get heat index data from the server
      getData("readCalculationResults", "calculationResults");  // Get calculation results from the server
    }, 1000);

    /* 
    Function to fetch data from the server 
    url: the server endpoint to fetch data from
    elementId: the id of the HTML element to update with the fetched data
    */
    function getData(url, elementId) {
      var xhr = new XMLHttpRequest();  // Create a new XMLHttpRequest object
      xhr.open("GET", url, true);      // Initialize a GET request to the specified URL
      xhr.onreadystatechange = function () {  // Define a callback function to handle the response
        if (xhr.readyState == 4 && xhr.status == 200) {  // Check if the response is ready and OK
          document.getElementById(elementId).innerHTML = xhr.responseText;  // Update the element's content
        }
      };
      xhr.send();  // Send the request to the server
    }
  </script>
</html>
)=====";
