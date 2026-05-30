const main = document.getElementById("main")
const monthYear = document.getElementById("month-year")
const months = ["January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"]
const daysContainer = document.getElementById("days")
const prevButton = document.getElementById("prev")
const nextButton = document.getElementById("next")
const heading = document.querySelector(".popup h3")
const recButton = document.getElementById("recommend")

let numberOfDays = null
let dayButtons = null

let currentDate = new Date()
let today = new Date()

function popup(dayButton) {
    heading.textContent = dayButton.textContent + " " + monthYear.textContent
}

function renderCalendar(date) {
    const year = date.getFullYear()
    const month = date.getMonth()
    const firstDay = new Date(year, month, 1).getDay()
    const lastDay = new Date(year, month + 1, 0).getDate()

    monthYear.textContent = `${months[month]} ${year}`

    daysContainer.innerHTML = ""

    const prevMonthLastDay = new Date(year, month, 0).getDate()

    for (let i = firstDay; i > 0; i--) {
        const prevDay = document.createElement("h3")
        prevDay.textContent = prevMonthLastDay - i + 1
        prevDay.classList.add("fade", "day")
        daysContainer.appendChild(prevDay)
    }

    for (let i = 1; i <= lastDay; i++) {
        const dayText = document.createElement("h3")
        dayText.textContent = i
        dayText.classList.add("day")
        
        if (i === today.getDate() && month === today.getMonth() && year === today.getFullYear()) {
            dayText.classList.add("today")
        }
        daysContainer.appendChild(dayText)
    }

    const nextMonthStartDay = 7 - new Date(year, month + 1, 0).getDay() - 1

    for (let i = 1; i <= nextMonthStartDay; i++) {
        const nextDay = document.createElement("h3")
        nextDay.textContent = i
        nextDay.classList.add("fade", "day")
        daysContainer.appendChild(nextDay)
    }

    dayButtons = daysContainer.querySelectorAll(".day")

    dayButtons.forEach(button => {
        button.addEventListener("click", function () {
            popup(button)
        })
    })
}

prevButton.addEventListener("click", function () {
    currentDate.setMonth(currentDate.getMonth() - 1)
    renderCalendar(currentDate)
})

nextButton.addEventListener("click", function () {
    currentDate.setMonth(currentDate.getMonth() + 1)
    renderCalendar(currentDate)
})

renderCalendar(currentDate)
