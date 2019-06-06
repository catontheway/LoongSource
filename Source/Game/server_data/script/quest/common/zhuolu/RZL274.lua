
--12274���ڴ��[3020-3100,y,1513-1557]��ʹ����Ʒ����[2612148]���ٻ�2����ͥ����[1005140]��1����ͥ����[1005139]



--��Ʒ����[2612148] �ڴ��[3020-3100,y,1513-1557]֮�����겻����
function I2612148_CanUse(MapID, InstanceID, TypeID, RoleID)
	local bRet, bIgnore = 0, false
	--������¹����ʹ��
	if MapID ~= 3017299663 then
	    bRet = 43
	else
	    local x, y ,z = unit.GetPosition(MapID, InstanceID, RoleID)
        if x<=3020 or x>=3100 or z<= 1513 or z>=1557 then  --�����ڸ÷�Χ��ʹ��
            bRet = 43
		else
			if map.GetMapScriptData(MapID,InstanceID,1,1) >= 2 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 0)
				msg.AddMsgEvent(MsgID, 1, 148)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
				bRet = 32
			elseif map.GetMapScriptData(MapID,InstanceID,1,2) >= 1 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 0)
				msg.AddMsgEvent(MsgID, 1, 148)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
				bRet = 32
			end
        end
    end
	return bRet, bIgnore
end



--ʹ������[2612148]�ٻ�2����ͥ����[1005140]��1����ͥ����[1005139]
function I2612148_Use(MapID, InstanceID, TypeID, RoleID)
	--����1 ��12274��1005140����ͥ����
	--����2 ��12274��1005139����ͥ����
	local m = map.GetMapScriptData(MapID,InstanceID,1,1)
	local n = map.GetMapScriptData(MapID,InstanceID,1,2)

	if m == nil then
		m = 0
	end
	if n == nil then
		n = 0
	end

	local x, y ,z = unit.GetPosition(MapID, InstanceID, RoleID)
	map.MapCreateCreature(MapID, InstanceID, 1005140, x-2, y, z)
	map.MapCreateCreature(MapID, InstanceID, 1005140, x+2, y, z)
	map.MapCreateCreature(MapID, InstanceID, 1005139, x, y, z+2)

	map.SetMapScriptData(MapID,InstanceID,1,1,m+2)
	map.SetMapScriptData(MapID,InstanceID,1,2,n+1)

end

aux.RegisterItemEvent(2612148, 1, "I2612148_Use")
aux.RegisterItemEvent(2612148, 0, "I2612148_CanUse")

function c1005140_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	local a = map.GetMapScriptData(MapID,InstanceID,1,1)
	map.SetMapScriptData(MapID,InstanceID,1,1,a-1)

end
aux.RegisterCreatureEvent(1005140,4,"c1005140_OnDie")

function c1005139_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	local a = map.GetMapScriptData(MapID,InstanceID,1,2)
	map.SetMapScriptData(MapID,InstanceID,1,2,a-1)
end
aux.RegisterCreatureEvent(1005139,4,"c1005139_OnDie")
