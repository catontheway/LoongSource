--Ů����-Ҽ��10437
--����1������������<3351, 837>��<3371, 857>��Χ��ʹ����Ʒ������¶[2610170]��ˢ�³����ﶾ��Ů��[1003165]

--������¶��Ʒʹ���жϺ���
function i2610170_CanUse(MapID, InstanceID, TypeID, RoleID)
	local bRet, bIgnore = 0, false

	--����������ǳ���������ʹ��
	if MapID ~= 3017298127 then
	    bRet = 43
	else
	    --�õ���ҵ�ǰ����
	    local x, y ,z = unit.GetPosition(MapID, InstanceID, RoleID)
	    if x<=3351 or x>=3371 or z<=837 or z>=857 then  --�������������귶Χ������ʹ��
	        bRet = 43
		else
			if map.GetMapScriptData(MapID,InstanceID,1,2) >= 10 then
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

--������¶��Ʒʹ��Ч������
function i2610170_QuestUsable(MapID, InstanceID, TypeID, TargetID)
	--m02��ͼ����2��10437 ����Ů��[1003165]��¼����
	local a = map.GetMapScriptData(MapID,InstanceID,1,2)
	if a == nil then
		a = 0
	end
	if a < 10 then
		local x, y ,z = unit.GetPosition(MapID, InstanceID, TargetID)
		map.MapCreateCreature(MapID, InstanceID, 1003165, x+2, y, z+2)
		map.SetMapScriptData(MapID,InstanceID,1,2,a+1)
	end

end

--ע��
aux.RegisterItemEvent(2610170, 1, "i2610170_QuestUsable")
aux.RegisterItemEvent(2610170, 0, "i2610170_CanUse")

--������������ScriptData����2 ��ֵ��1


function c1003165_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	local a = map.GetMapScriptData(MapID,InstanceID,1,2)
	map.SetMapScriptData(MapID,InstanceID,1,2,a-1)
end
aux.RegisterCreatureEvent(1003165,4,"c1003165_OnDie")
