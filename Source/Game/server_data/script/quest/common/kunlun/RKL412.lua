--15412ʹ�õ�����аͷ��2615207����ѩ��կ�ٻ�BOSSѩ����1009234��x1330 y17622 z881

--�ɷ�ʹ�ú���
function i2615207_CanUse(MapID, InstanceID, TypeID, TargetID)
    local bRet, bIgnore = 0, false
	--����������أ�����ʹ��
	if MapID ~= 3017299407 then
	    return 43, false
	else
	--�������ָ���ص㣬����ʹ��
	    local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	    if x < 1230 or x > 1430 or z < 781 or z > 981 then
	          return 43, false 
            end
	end
    return bRet, bIgnore
end

--ʹ��Ч��
function i2615207_QuestUsable(MapID, InstanceID, TypeID, TargetID)
       map.MapCreateCreature(MapID, InstanceID, 1009234, 1330, 17622, 881)
end

--ע��
aux.RegisterItemEvent(2615207, 1, "i2615207_QuestUsable")
aux.RegisterItemEvent(2615207, 0, "i2615207_CanUse")