TButton(id="infoButton", sizeX="0.05", sizeY="0.1", posX="0.9", posY="0.05", text="?", texture="Resources/Images/button.png", clickSound="Resources/Audio/click.wav", hoverSound="Resources/Audio/hover.wav")

TContainer(id="menu", sizeX="0.8", sizeY="0.8", posX="0.1", posY="0.1", title="Main Menu", drawOutline="0", startHidden="0")
TButton(id="startGame", sizeX="0.2", sizeY="0.1", posX="0.4", posY="0.5", text="", texture="Resources/Images/startButton.png", clickSound="Resources/Audio/click.wav", hoverSound="Resources/Audio/hover.wav")
TButton(id="settingsButton", sizeX="0.2", sizeY="0.1", posX="0.4", posY="0.35", text="", texture="Resources/Images/settingsButton.png", clickSound="Resources/Audio/click.wav", hoverSound="Resources/Audio/hover.wav")
TButton(id="exitGame", sizeX="0.2", sizeY="0.1", posX="0.4", posY="0.20", text="", texture="Resources/Images/exitButton.png", clickSound="Resources/Audio/click.wav", hoverSound="Resources/Audio/hover.wav")
TLabel(id="titleLabel", sizeX="0.7", sizeY="0.3", posX="0.15", posY="0.75", ptSize="100", text="Nuclear Throne!")
TEndContainer()

TContainer(id="settings", sizeX="0.8", sizeY="0.8", posX="0.1", posY="0.1", title="Settings", drawOutline="0", startHidden="1")
TLabel(id="titleLabel", sizeX"0.7", sizeY="0.3", posX="0.15", posY="0.75", ptSize="100", text="Nuclear Throne!")
TLabel(id="subTitleLabel", sizeX"0.2", sizeY="0.1", posX="0.1", posY="0.65", ptSize="50", text="Settings:")
TEndContainer()

TContainer(id="charSelection", sizeX="0.8", sizeY="0.8", posX="0.1", posY="0.1", title="Character Selection", drawOutline="0", startHidden="1")
TButton(id="next", sizeX="0.2", sizeY="0.1", posX="0.4", posY="0.20", text=">", texture="Resources/Images/button.png", clickSound="Resources/Audio/click.wav", hoverSound="Resources/Audio/hover.wav")
TEndContainer()