--Ů����-����10438
--����1������������<3584, 639>�뾶30��Χ��ʹ����Ʒ������¶[2610171]��ˢ�³������ˮ��Ů��[1003166]

--������¶�ɷ�ʹ�ú���
function i2610171_CanUse(MapID, InstanceID, TypeID, RoleID)
    local bRet, bIgnore = 0, false

	--����������ǳ���������ʹ��
	if MapID ~= 3017298127 then
	    bRet = 43
	else
	    --�õ���ҵ�ǰ����
	    local x, y ,z = unit.GetPosition(MapID, InstanceID, RoleID)
	    if x<=3554 or x>=3614 or z<=609 or z>=669 then  --�������������귶Χ������ʹ��
	        bRet = 43
		else
			if map.GetMapScriptData(MapID,InstanceID,1,3) >= 10 then
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

--������¶ʹ��Ч��
function i2610171_QuestUsable(MapID, InstanceID, TypeID, TargetID)
	--m02��ͼ����3��10438 ��ˮ��Ů��[1003166]��¼����
	local a = map.GetMapScriptData(MapID,InstanceID,1,3)
	if a == nil then
		a = 0
	end
	if a < 10 then
		local x, y ,z = unit.GetPosition(MapID, InstanceID, TargetID)
		map.MapCreateCreature(MapID, InstanceID, 1003166, x+2, y, z+2)
		map.SetMapScriptData(MapID,InstanceID,1,3,a+1)
	end
end

--ע��
aux.RegisterItemEvent(2610171, 1, "i2610171_QuestUsable")
aux.RegisterItemEvent(2610171, 0, "i2610171_CanUse")

--������������ScriptData����3 ��ֵ��1


function c1003166_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	local a = map.GetMapScriptData(MapID,InstanceID,1,3)
	map.SetMapScriptData(MapID,InstanceID,1,3,a-1)
end
aux.RegisterCreatureEvent(1003166,4,"c1003166_OnDie")
