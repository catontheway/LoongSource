--��ţ֮��13449
--����1����ţ��ʯ��̳[m05: 1821, 2555]����ʹ����Ʒţ��ʯ��Ʒ[2613226]��ˢ�¹�����ţ��[1006321]

--��Ʒţ��ʯʹ���жϽű�
function i2613226_CanUse(MapID, InstanceID, TypeID, RoleID)
	local bRet, bIgnore = 0, false

	--������ˮ����ʹ��
	if MapID ~= 3017299919 then
	    bRet = 43
	else
		local x, y ,z = unit.GetPosition(MapID, InstanceID, RoleID)
	    if x<=1806 or x>=1838 or z<=2539 or z>=2571 then
		    bRet = 43
		else
			if map.GetMapScriptData(MapID,InstanceID,1,1) >= 10 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 0)
			msg.AddMsgEvent(MsgID, 1, 148)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
			bRet = 32
			end
	    end
	end

	--����
	return bRet, bIgnore
end

--��Ʒţ��ʯʹ��Ч���ű�
function i2613226_QuestUsable(MapID, InstanceID, TypeID, TargetID)
	--m05��ͼ����1��13449 ��ţ��[1006321]��¼����
	local a = map.GetMapScriptData(MapID,InstanceID,1,1)
	if a == nil then
		a = 0
	end
	if a < 10 then
		local x, y ,z = unit.GetPosition(MapID, InstanceID, TargetID)
		map.MapCreateCreature(MapID, InstanceID, 1006321, 1822, 24427, 2555)
		map.SetMapScriptData(MapID,InstanceID,1,1,a+1)
	end
end

--ע��
aux.RegisterItemEvent(2613226, 1, "i2613226_QuestUsable")
aux.RegisterItemEvent(2613226, 0, "i2613226_CanUse")

function c1006321_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	local a = map.GetMapScriptData(MapID,InstanceID,1,1)
	map.SetMapScriptData(MapID,InstanceID,1,1,a-1)
end
aux.RegisterCreatureEvent(1006321,4,"c1006321_OnDie")
