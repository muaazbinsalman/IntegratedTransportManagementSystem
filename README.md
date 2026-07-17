# 🚆 Integrated Transport Management System

## 📌 About
**Integrated Transport Management System** is a full‑stack transport booking solution designed to streamline journey planning and reservation. This project offers a sleek, responsive frontend combined with a robust C++ CGI backend that handles dynamic passenger input, station selection, fare calculations (with discount conditions based on journey time), and real‑time booking confirmations—all displayed on one beautifully styled page.

Built using:
- **Frontend:** HTML5, CSS3, JavaScript (AJAX via the Fetch API)
- **Backend:** C++ CGI scripting (compiled into an executable)
- **Server:** Apache (with CGI enabled via `.htaccess`)

## 🚀 Key Features
- **Dynamic Passenger Input:** Automatically generates the required input fields based on the number of passengers.
- **Time Validation:** Enforces HH:MM format using an HTML5 time input with backend validation to ensure hours are 0–23 and minutes 0–59.
- **Station Selection & Train Choice:** List of fixed stations and the option to choose between two train types.
- **Automated Fare Calculation:** Computes total fare with a 40% discount if the journey time falls within a preset discount window.
- **AJAX-Based Interaction:** Submits booking details via AJAX to display confirmation or error messages on the same, sleek page.
- **Sleek, Modern UI:** Designed with a contemporary dark theme and blue call-to-action buttons for an elegant user experience.

## 🛠 Installation

### 1️⃣ Clone the Repository
```bash
git clone https://github.com/your-username/IntegratedTransportSystem.git
cd IntegratedTransportSystem
```

### 2️⃣ Directory Structure
```
IntegratedTransportSystem/
├── index.html                # Frontend UI
├── transport.cpp             # Combined C++ CGI source code
├── transport.cgi             # Compiled CGI executable
└── .htaccess                 # Apache configuration file for CGI execution
```

### 3️⃣ Compile the CGI Source Code
Open a command prompt in the project directory and run:
```bash
g++ transport.cpp -o transport.cgi -O2
```

### 4️⃣ Configure Apache
Place the repository folder within your Apache server’s DocumentRoot (e.g., `C:/xampp/htdocs/IntegratedTransportSystem/`) ensuring the `.htaccess` file is present.

### 5️⃣ Restart Apache
Restart Apache via your XAMPP Control Panel to apply all settings.

### 6️⃣ Launch the Application
Open your web browser and navigate to:
```
http://localhost/IntegratedTransportSystem/index.html
```
Fill out the booking form and click **Book Now** to receive your booking confirmation on the same page.

## ✨ Usage Guide
1. **Journey Details:**  
   - Enter the number of passengers.
   - Select a valid journey time in HH:MM format using the time input.
   - Choose your starting and destination stations from the provided list.
2. **Train Selection:**  
   - Pick between “Express Train” and “Local Train.”
3. **Passenger Information:**  
   - Dynamic fields will be generated for each passenger.
4. **Submit Your Booking:**  
   - Click the blue **Book Now** button.
   - Your booking confirmation, inclusive of fare details and discount status, will appear without refreshing the page.

## 👥 Contributors
- **Main Lead:** Muaaz Bin Salman  
- **Co-Authors:** Momna Jamil, Sofia Faisal, Rabiya Tahir

## 🎯 Contributing
Contributions, improvements, or feedback are highly welcome. To contribute:
- Fork this repository.
- Create your feature branch (e.g., `git checkout -b feature/your-feature`).
- Commit your changes with clear messages.
- Push your branch and open a pull request.

## 📖 License
This project is licensed under the **MIT License**. See the [LICENSE](LICENSE) file for details.

