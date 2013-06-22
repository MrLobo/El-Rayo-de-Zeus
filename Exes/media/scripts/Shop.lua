-- Fichero Shop.lua

-- Vars initialization showed in SHOP

local area

--Funcion de inicializacion/carga de un layout y sus ventanas y subventanas
function initShop()

	if shop==nil then
	
		--GUI.Windows:loadWindowLayout("Shop.layout")
		--shop = GUI.Windows:getWindow("Shop")
		
		root = GUI.Windows:getWindow("Root")
		
		shop = GUI.Windows:getWindow("Root/Shop")
		
		pestanas = GUI.Windows:getWindow("Root/Shop/ControlPestanas")
		upgrades = GUI.Windows:getWindow("Root/Shop/ControlPestanas/Upgrades")
		allies = GUI.Windows:getWindow("Root/Shop/ControlPestanas/Allies")
		
		pmShop = GUI.Windows:getWindow("Root/Shop/PM")
		
		--descriptionItem = GUI.Windows:getWindow("Root/Shop/ControlPestanas/Upgrades/Item1/Description")
		
		--costItem = GUI.Windows:getWindow("Root/Shop/ControlPestanas/Upgrades/Item1/costItem1")
		
		--area = shop:getInnerRectClipper() -- area es una variable de tipo Rect

		--tabControlWindow = shop:getChild("Shop/TabControl")
		
		--local tc = CEGUI.toTabControl(tabControlWindow)

        -- Add some pages to tab control
		--tc:addTab (GUI.Windows:loadWindowLayout("TabPageCriatures.layout"))	
        --tc:addTab (GUI.Windows:loadWindowLayout("TabPageItems.layout"))
			

	end
end


--Funcion que muestra la ventana del Shop
function showShop()
	--if shop ~= nil then
		--GUI.System:setGUISheet(shop)
		--shop:setVisible(true)
		--shop:activate()
	--end
	if root ~= nil then
		GUI.System:setGUISheet(root)
		shop:setVisible(true)
		shop:activate()
		GUI.Mouse:show()
	end	
end


--Funcion que oculta una ventana del GUI
function hideShop()
	--if shop ~= nil then
		--shop:deactivate()		
		--shop:setVisible(false)
	--end
    if root ~= nil then
		shop:deactivate()		
		shop:setVisible(false)
		GUI.Mouse:hide()
	end	
end


function changeShopVisibility()
		if shop:isVisible() then
			shop:setVisible(false)
			GUI.Mouse:hide()
		else
			shop:setVisible(true)
			GUI.Mouse:show()
			--GUI.Mouse:setUnifiedConstraintArea(nil)
		end
end

function showPMSHop(pm)
	pmShop:setText(pm)
end

function reloadShop()
	if shop ~= nil then
		shop:setProperty("UnifiedAreaRect","{{0.727354,0},{0.00944353,0},{0.997086,0},{0.982883,0}}")
		--descriptionItem:setProperty("Font","DejaVuSans-6bold")
		--costItem:setProperty("Font","DejaVuSans-6bold")
		
		--pestanas:setProperty("UnifiedAreaRect","{{0.0242733,0},{-0.00190571,0},{1.00578,0},{0.882612,0}}")
		pestanas:setProperty("UnifiedAreaRect","{{0.01,0},{0,0},{0.99,0},{0.9,0}}")
		upgrades:setProperty("UnifiedAreaRect","{{0,0},{0,0},{0.99,0},{0.8,0}}")
		allies:setProperty("UnifiedAreaRect","{{0,0},{0,0},{0.99,0},{0.8,0}}")
		
		--TitlePuntosMeritoWindow:setProperty("UnifiedAreaRect","{{0,105},{0.5,0},{0,150},{0.7,0}}")
		--PuntosMeritoWindow:setProperty("UnifiedAreaRect","{{0,150},{0.5,0},{0,210},{0.7,0}}")
	end
end

