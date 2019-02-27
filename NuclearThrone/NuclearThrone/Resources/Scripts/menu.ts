TButton(id="infoButton", sizeX="0.05", sizeY="0.1", posX="0.9", posY="0.05", text="?", texture="Resources/Images/button.png")

TContainer(id="menu", sizeX="0.8", sizeY="0.8", posX="0.1", posY="0.1", title="Main Menu", drawOutline="0", startHidden="0")
TButton(id="startGame", sizeX="0.2", sizeY="0.1", posX="0.4", posY="0.6", text="Start Game", texture="Resources/Images/button.png")
TButton(id="settingsButton", sizeX="0.2", sizeY="0.1", posX="0.4", posY="0.45", text="Settings", texture="Resources/Images/button.png")
TButton(id="exitGame", sizeX="0.2", sizeY="0.1", posX="0.4", posY="0.30", text="Exit", texture="Resources/Images/button.png")
TLabel(id="titleLabel", sizeX="0.7", sizeY="0.3", posX="0.15", posY="0.75", ptSize="100", text="Nuclear Throne!")
TEndContainer()

TContainer(id="settings", sizeX="0.8", sizeY="0.8", posX="0.1", posY="0.1", title="Settings", drawOutline="0", startHidden="1")
TLabel(id="titleLabel", sizeX"0.7", sizeY="0.3", posX="0.15", posY="0.75", ptSize="100", text="Nuclear Throne!")
TLabel(id="subTitleLabel", sizeX"0.2", sizeY="0.1", posX="0.1", posY="0.65", ptSize="50", text="Settings:")
TEndContainer()

TContainer(id="pauseMenu", sizeX="0.8", sizeY="0.8", posX="0.1", posY="0.1", title="Settings", drawOutline="0", startHidden="1")
TButton(id="resumeGame", sizeX="0.2", sizeY="0.1", posX="0.4", posY="0.6", text="Resume!", texture="Resources/Images/button.png")
TButton(id="exitMainScreen", sizeX="0.2", sizeY="0.1", posX="0.4", posY="0.6", text="Exit to main screen", texture="Resources/Images/button.png")
TEndContainer()
