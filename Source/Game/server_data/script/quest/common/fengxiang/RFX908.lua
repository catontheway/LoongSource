--RFX908
--�������þ���1826990375���������龫�ǣ�3011008��ˢ���ҷ�ս�꣨1531006��
--map.SetMapScriptData(MapID, InstanceID, 1, 2, 0)�ڼ��ε������龫�ǣ�ÿ��%4����Ϊ3��ʱ��ˢ���ҷ�ս��
--map.SetMapScriptData(MapID, InstanceID, 1, 1, 0)�ڼ��δ������龫�ǣ������δ������Ѫ��ʯ��˫���δ������̫��ʯ�ͷ���ʯ��Ч��
--�������龫��
function c3011008_OnCheck(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
       local m = map.GetMapScriptData(MapID, InstanceID, 1, 1)
       local n = map.GetMapScriptData(MapID, InstanceID, 1, 2)
       local i = m%2
       if i==0 then
           unit.AddBuff(MapID, InstanceID, RoleID, 2016901, RoleID)
       elseif i==1 then
           unit.AddBuff(MapID, InstanceID, RoleID, 2017001, RoleID)
           unit.AddBuff(MapID, InstanceID, RoleID, 2017101, RoleID)
       end
       local k = n%3
       if k == 2 or n == 0 then 
           map.MapCreateCreature(MapID, InstanceID, 1531006, 216, 160, 233)
       end
       map.SetMapScriptData(MapID, InstanceID, 1, 1, m+1)
       map.SetMapScriptData(MapID, InstanceID, 1, 2, n+1)
end

aux.RegisterCreatureEvent(3011008, 6, "c3011008_OnCheck")


--�����ȡʱ����
--function x11908_OnAccept(MapID, InstanceID, questID, ownerID, accepterID)
--       role.RoleGotoNewMap(MapID, InstanceID, ownerID, 1826990375, 171, 5, 32)
--end

--ע��
--aux.RegisterQuestEvent(11908, 0, "x11908_OnAccept")


