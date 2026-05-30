const main = document.getElementById("main")
const indexes = ["Hat.png"]
const selected = null


indexes.forEach(i => {
    const container = document.createElement("article")
    container.classList.add("image-card")

    const image = document.createElement("img")
    image.src = `/images/${i}`
    image.id = i.split(".")[0]
    container.appendChild(image)

    const text = document.createElement("p")
    text.textContent = i.split(".")[0]
    text.id = i.split(".")[0] + "-text"
    container.appendChild(text)

    main.appendChild(container)
})