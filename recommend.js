const recButton = document.getElementById("recommend")
const recommendSection = document.getElementById("recommend-section")

recButton.addEventListener("click", function () {
    const popupOverlay = document.createElement("section")
    popupOverlay.id = "popup-overlay"
    recommendSection.appendChild(popupOverlay)

    const recommendPopup = document.createElement("section")
    recommendPopup.id = "recommend-popup"
    popupOverlay.appendChild(recommendPopup)

    const recommendHeading = document.createElement("h3")
    recommendHeading.textContent = "Loading recommendations..."
    recommendPopup.appendChild(recommendHeading)

    const throbber = document.createElement("span")
    throbber.classList.add("loader")
    recommendPopup.appendChild(throbber)

    setTimeout(function () {
        throbber.remove()
        recommendHeading.textContent = "Recommendations: Hat"

        const image = document.createElement("img")
        image.src = "images/Hat.png"
        image.id = "recommend-img"
        recommendPopup.appendChild(image)

        const para = document.createElement("p")
        para.textContent = "AI Explanation: The event you are attending today is a HSBC Marathon with sunny skies and temperatures of 28 degrees, therefore a hat would be recommended."
        recommendPopup.appendChild(para)

        const exit = document.createElement("button")
        exit.textContent = "X"
        recommendPopup.appendChild(exit)
    }, 2000)
})