--15408ʹ�õ��������2615229������ѩ��կ�Ŀ������ٻ�NPCһü���� ��3070209��(1222,17387,874)һü����ˢ������Ϊ1��

--�ɷ�ʹ�ú���
function i2615229_CanUse(MapID, InstanceID, TypeID, TargetID)
    local bRet, bIgnore = 0, false
	--����������أ�����ʹ��
	if MapID ~= 3017299407 then
	    return 43, false
	else
	--�������ָ���ص㣬����ʹ��
	    local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	    if x < 1152 or x > 1292 or z < 804 or z > 944 then
	          return 43, false 
            end
	end
	local m = map.GetMapScriptData(MapID, InstanceID, 1, 0)
	if m == 1 then 
	    return 32, false
	end
    return bRet, bIgnore
end

--ʹ��Ч��
function i2615229_QuestUsable(MapID, InstanceID, TypeID, TargetID)
       map.MapCreateCreature(MapID, InstanceID, 3070209, 1222, 17387, 874)
       map.SetMapScriptData(MapID, InstanceID, 1, 0, 1)
end

--ע��
aux.RegisterItemEvent(2615229, 1, "i2615229_QuestUsable")
aux.RegisterItemEvent(2615229, 0, "i2615229_CanUse")

--һü������ʧʱ����־λ��0
function c3070209_OnDisappear(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
       map.SetMapScriptData(MapID, InstanceID, 1, 0, 0)
end
--ע��
aux.RegisterCreatureEvent(3070209, 13, "c3070209_OnDisappear")