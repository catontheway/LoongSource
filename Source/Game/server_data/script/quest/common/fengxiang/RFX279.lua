--�Ҿ�������11279
--����1���ڻ��κ���̳[m03: 2056, 910]����ʹ����Ʒ�Ҿ�[2611410]��ˢ�¹������ͯ��[1004607]

--��Ʒ�Ҿ�ʹ��Ч���ű�����
function i2611410_QuestUsable(MapID, InstanceID, TypeID, TargetID)
	--m03��ͼ����1��11279 ����ͯ��[1004607]��¼����
	local a = map.GetMapScriptData(MapID,InstanceID,1,1)
	if a == nil then
		a = 0
	end
	if a < 3 then
		local x, y ,z = unit.GetPosition(MapID, InstanceID, TargetID)
		map.MapCreateCreature(MapID, InstanceID, 1004607, x+2, y, z+2)
		map.SetMapScriptData(MapID,InstanceID,1,1,a+1)
	end
end

--��Ʒ�Ҿ�ʹ���жϽű�����
function i2611410_CanUse(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false

	--�����ڷ���ʹ��
	if MapID ~= 3017298383 then
	    bRet = 43
	else
		local x, y ,z = unit.GetPosition(MapID, InstanceID, TargetID)
	    if x<=2040 or x>=2072 or z<=894 or z>=926 then --�����ڸ�λ�ø���ʹ��
	        bRet = 43
		else
			if map.GetMapScriptData(MapID,InstanceID,1,1) >= 3 then
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

--ע��
aux.RegisterItemEvent(2611410, 1, "i2611410_QuestUsable")
aux.RegisterItemEvent(2611410, 0, "i2611410_CanUse")

--������ʧ������������ScriptData���� ��ֵ��1


function c1004607_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	local a = map.GetMapScriptData(MapID,InstanceID,1,1) --����Data����1
	map.SetMapScriptData(MapID,InstanceID,1,1,a-1)
end
aux.RegisterCreatureEvent(1004607,4,"c1004607_OnDie")
