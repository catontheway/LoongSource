Treasure_BeginRate 		= 	0			-- ��ʼ����
Treasure_RoleRate		= 	1			-- ��ɫ�ڵ����
Treasure_ServerRate		= 	2			-- �������ڵ����
Treasure_NormalRate		=	3			-- ��ͨ����

-- �����������
math.randomseed(os.time())

-- �õ���ǰ��������Ʒ����
function Treasure_GetRate(MapID, InstanceID, RoleID)

	local RoleChestSum, ServerChestSum = role.GetTreasureSum(MapID, InstanceID, RoleID)

	if ServerChestSum == 1000 then
		return Treasure_ServerRate

	elseif RoleChestSum == 100 then
		return Treasure_RoleRate

	elseif RoleChestSum <= 5 then
		return Treasure_BeginRate

	else
		return Treasure_NormalRate
	end
end

-- ��������
function Treasure_OpenChest(MapID, InstanceID, RoleID, ChestID, KeyID)

	local bOpened = false
	local ErrorCode = 0

	if role.CanOpenChest(MapID, InstanceID, RoleID, ChestID, KeyID) == true then
		if role.GetBagFreeSize(RoleID) == 0 then	-- �����ռ䲻��
			bOpened = false
			ErrorCode = -100
		else
			bOpened = true

			-- �����еı����Կ������1
			role.RemoveFromRole(MapID, InstanceID, RoleID, ChestID, 1, 300)
			role.RemoveFromRole(MapID, InstanceID, RoleID, KeyID, 1, 300)
			-- �����ҵ���
			local ret = role.IsRoleOnLine(MapID, InstanceID, RoleID)
			if ret == 0 then
				ErrorCode = -301
			else
				ErrorCode = 0
				-- �����������1
				role.IncTreasureSum(MapID, InstanceID, RoleID)
				local Rate = Treasure_GetRate(MapID, InstanceID, RoleID)
				role.SetRoleScriptData(RoleID, 1, RoleDataType["Chest_Rate"], Rate)
				role.SetRoleScriptData(RoleID, 1, RoleDataType["Chest_Step"], 1)
			end
		end

	else
		bOpened = false
	end

	-- ������Ϣ֪ͨ�ͻ���
	role.SendChestMsg(MapID, InstanceID, RoleID, ChestID, "OpenChest", bOpened, false, 0, 0, ErrorCode)
end


-- ֹͣ������������Ʒ
function Treasure_StopChest(MapID, InstanceID, RoleID, ChestID, KeyID)
	local ErrorCode = 0


	-- �����0��1֮��������������0
	local RandNum = math.random() + 0.0000001
	local Rate = role.GetRoleScriptData(RoleID, 1, RoleDataType["Chest_Rate"])
	local Step = role.GetRoleScriptData(RoleID, 1, RoleDataType["Chest_Step"])

	-- �����ҵ���
	local ret = role.IsRoleOnLine(MapID, InstanceID, RoleID)
	if ret == 0 then
		ErrorCode = -301
	else
		ErrorCode = 0
	end

	-- �õ������������Ʒid������
	local ItemID, Num = role.GetChestItem(MapID, InstanceID, RoleID, ChestID, Rate, RandNum)
	if ItemID == nil or Num == nil or (Step ~= 1 and Step ~= 3) then
		ErrorCode = -2
	else
	    if Step == 1 then
	        role.SetRoleScriptData(RoleID, 1, RoleDataType["Chest_Step"], 2)
		elseif Step == 3 then
	        role.SetRoleScriptData(RoleID, 1, RoleDataType["Chest_Step"], 4)
		else
		   ErrorCode = -2
		end
	end

	-- ������Ϣ���ͻ���
	role.SendChestMsg(MapID, InstanceID, RoleID, ChestID, "StopChest", true, false, ItemID, Num, ErrorCode)

end

-- ����һ������ѡȡ��Ʒ
function Treasure_AgainChest(MapID, InstanceID, RoleID)
	local ErrorCode = 0
	local Step = role.GetRoleScriptData(RoleID, 1, RoleDataType["Chest_Step"])
	-- 50%������
	local rand = math.random(0, 1)
    local bDestroy = true
	-- �����ҵ���
	local ret = role.IsRoleOnLine(MapID, InstanceID, RoleID)
	if ret == 0 or Step ~= 2 then
		ErrorCode = -301
	else
		ErrorCode = 0
    	if rand == 0 then
            bDestroy = false
            role.SetRoleScriptData(RoleID, 1, RoleDataType["Chest_Step"], 3)
        else
            bDestroy = true
            role.SetRoleScriptData(RoleID, 1, RoleDataType["Chest_Step"], 5)
        end
	end




	-- ������Ϣ���ͻ���
	role.SendChestMsg(MapID, InstanceID, RoleID, ChestID, "AgainChest", true, bDestroy, 0, 0, ErrorCode)

end

-- �õ������Ʒ��������������
function Treasure_GetItem(MapID, InstanceID, RoleID, ChestID, ItemID, ItemNum)
	local ErrorCode = 0
	-- �����ҵ���
	local ret = role.IsRoleOnLine(MapID, InstanceID, RoleID)
	local Step = role.GetRoleScriptData(RoleID, 1, RoleDataType["Chest_Step"])
	if ret == 0 or (Step ~= 2 and Step ~= 4)then
		ErrorCode = -301
	else
	    role.SetRoleScriptData(RoleID, 1, RoleDataType["Chest_Step"], 5)
		ErrorCode = 0
		role.AddRoleItem(MapID, InstanceID, RoleID, ItemID, ItemNum, -1, 3, 104)
		local needbroadcast = role.ItemNeedBroadcast(ItemID)
		if needbroadcast == 1 then
			-- �㲥��Ʒ
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 100, 280)
			msg.AddMsgEvent(MsgID, 2, RoleID)
			msg.AddMsgEvent(MsgID, 4, ChestID)
			msg.AddMsgEvent(MsgID, 4, ItemID)
			msg.DispatchWorldMsgEvent(MsgID)
		end
	end

	-- ������Ϣ���ͻ���
	role.SendChestMsg(MapID, InstanceID, RoleID, ChestID, "GetItem", true, false, ItemID, ItemNum, ErrorCode)

end

aux.RegisterRoleEvent(7, "Treasure_OpenChest")
aux.RegisterRoleEvent(8, "Treasure_StopChest")
aux.RegisterRoleEvent(9, "Treasure_AgainChest")
aux.RegisterRoleEvent(10, "Treasure_GetItem")

