--Ů����-����10439��
--����1������������<3488, 791>��Χ��ʹ����Ʒ��¶����[2610172]��ˢ�³�����޷�Ů��[1003167]

--��¶������Ʒʹ���жϺ���
function i2610172_CanUse(MapID, InstanceID, TypeID, RoleID)
    local bRet, bIgnore = 0, false

	--����������ǳ���������ʹ��
	if MapID ~= 3017298127 then
	    bRet = 43
	else
	    --�õ���ҵ�ǰ����
	    local x, y ,z = unit.GetPosition(MapID, InstanceID, RoleID)

	    if x<=3448 or x>=3538 or z<=751 or z>=831 then  --�������������귶Χ������ʹ��
	        bRet = 43
		else
			if map.GetMapScriptData(MapID,InstanceID,1,4) >= 10 then
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

--��¶����ʹ��Ч������
function i2610172_QuestUsable(MapID, InstanceID, TypeID, TargetID)
--m02��ͼ����4��10439 �޷�Ů��[1003167]��¼����
	local a = map.GetMapScriptData(MapID,InstanceID,1,4)
	if a == nil then
		a = 0
	end
	if a < 10 then
		local x, y ,z = unit.GetPosition(MapID, InstanceID, TargetID)
		map.MapCreateCreature(MapID, InstanceID, 1003167, x+2, y, z+2)
		map.SetMapScriptData(MapID,InstanceID,1,4,a+1)
	end
end

--ע��
aux.RegisterItemEvent(2610172, 1, "i2610172_QuestUsable")
aux.RegisterItemEvent(2610172, 0, "i2610172_CanUse")

function c1003167_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	local a = map.GetMapScriptData(MapID,InstanceID,1,4)
	map.SetMapScriptData(MapID,InstanceID,1,4,a-1)
end
aux.RegisterCreatureEvent(1003167,4,"c1003167_OnDie")
